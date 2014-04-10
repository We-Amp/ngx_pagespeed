/*
 * Copyright 2014 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// Author: kspoelstra@we-amp.com (Kees Spoelstra)

#include "ngx_gzip_setter.h"

#include <ngx_conf_file.h>

namespace net_instaweb {

// TODO(kspoelstra): Could be moved to a pagespeed module context.
NgxGZipSetter g_gzip_setter;

extern "C" {
  // These functions replace the setters for
  // gzip
  // gzip_types
  // gzip_http_version
  // gzip_vary
  // If these functions are called it means there is an explicit gzip
  // configuration. The gzip configuration set by pagespeed is then rollbacked
  // and pagespeed will stop enabling gzip automatically.
  char* ngx_gzip_redirect_conf_set_flag_slot(
      ngx_conf_t* cf, ngx_command_t* cmd, void* conf) {
    if (g_gzip_setter.enabled()) {
      g_gzip_setter.RollBackAndDisable();
    }
    char* ret = ngx_conf_set_flag_slot(cf, cmd, conf);
    return ret;
  }

  char* ngx_gzip_redirect_http_types_slot(
      ngx_conf_t* cf, ngx_command_t* cmd, void* conf) {
    if (g_gzip_setter.enabled()) {
      g_gzip_setter.RollBackAndDisable();
    }
    char* ret = ngx_http_types_slot(cf, cmd, conf);
    return ret;
  }

  char* ngx_gzip_redirect_conf_set_enum_slot(
        ngx_conf_t* cf, ngx_command_t* cmd, void* conf) {
      if (g_gzip_setter.enabled()) {
        g_gzip_setter.RollBackAndDisable();
      }
      char* ret = ngx_conf_set_enum_slot(cf, cmd, conf);
      return ret;
  }
  char* ngx_gzip_redirect_conf_set_bitmask_slot(
          ngx_conf_t* cf, ngx_command_t* cmd, void* conf) {
        if (g_gzip_setter.enabled()) {
          g_gzip_setter.RollBackAndDisable();
        }
        char* ret = ngx_conf_set_bitmask_slot(cf, cmd, conf);
        return ret;
    }
}

NgxGZipSetter::NgxGZipSetter()
    : enabled_(0) {
}

NgxGZipSetter::~NgxGZipSetter() {
}

// Helper functions to determine signature
bool HasLocalConfig(ngx_command_t* command) {
  return !(command->type&(NGX_DIRECT_CONF|NGX_MAIN_CONF)) &&
                command->conf == NGX_HTTP_LOC_CONF_OFFSET;
}

bool IsNgxFlagCommand(ngx_command_t* command) {
  return (
      command->set == ngx_conf_set_flag_slot &&
      HasLocalConfig(command));
}

bool IsNgxHttpTypesCommand(ngx_command_t* command) {
  return (command->set == ngx_http_types_slot &&
              HasLocalConfig(command));
}
bool IsNgxEnumCommand(ngx_command_t* command) {
  return (command->set == ngx_conf_set_enum_slot &&
              HasLocalConfig(command));
}

bool IsNgxBitmaskCommand(ngx_command_t* command) {
  return (command->set == ngx_conf_set_bitmask_slot &&
              HasLocalConfig(command));
}

// Initialize the NgxGzipSetter.
// Find the gzip, gzip_vary, gzip_http_version and gzip_types commands in the
// gzip module. Enable if the signature of the zip command matches with what we
// trust. Also sets up redirects for the configurations. These redirect handle
// a rollback if expicit configuration is found.
// Returns:
//
// kInitGZipOk
// gzip, gzip_vary, gzip_http_version and
// gzip_types are found and signatures are ok.
// gzip will be enabled.
//
// kInitGZipSecondarySignatureMismatch
// gzip is ok, one of the other gzip functions has
// an unexpected signature.
// gzip will be partially enabled.
//
// kInitGZipSignatureMismatchFatal
// gzip has an unexpected signature.
// gzip will not be enabled.
//
// kInitGZipNotFound
// gzip command is not found.
// gzip will not be enabled.
//
// kInitGZipSecondaryMissing
// one or more of the secondary gzip functions is
// missing.
// gzip will be partially enabled
//
gzs_init_result NgxGZipSetter::Init() {
  int signature_mismatch_2nd = 0;
  int signature_mismatch_1st = 0;
  for (int m = 0; ngx_modules[m]; m++) {
    if (ngx_modules[m]->commands) {
      for (int c = 0; ngx_modules[m]->commands[c].name.len; c++) {
        ngx_command_t* current_command=&ngx_modules[m]->commands[c];

        // We look for the gzip command , and the exact signature we trust
        // this means configured as an config location offset
        // and a ngx_flag_t setter.
        // Also see:
        // ngx_conf_handler in ngx_conf_file.c
        // ngx_http_gzip_filter_commands in ngx_http_gzip_filter.c
        // Code below is very verbose
        if (!gzip_command_.command_ &&
            ngx_strcmp("gzip", current_command->name.data) == 0) {
          if (IsNgxFlagCommand(current_command)) {
            current_command->set = ngx_gzip_redirect_conf_set_flag_slot;
            gzip_command_.command_ = current_command;
            gzip_command_.module_ = ngx_modules[m];
            enabled_ = 1;
          } else {
            ngx_log_error(NGX_LOG_WARN, ngx_cycle->log, 0,
                          "Pagespeed: cannot set gzip, signature mismatch");
            signature_mismatch_1st++;
          }
        }

        if (!gzip_http_version_command_.command_ &&
            ngx_strcmp("gzip_http_version", current_command->name.data) == 0) {
          if (IsNgxEnumCommand(current_command)) {
            current_command->set = ngx_gzip_redirect_conf_set_enum_slot;
            gzip_http_version_command_.command_ = current_command;
            gzip_http_version_command_.module_ = ngx_modules[m];
          } else {
            ngx_log_error(NGX_LOG_WARN, ngx_cycle->log, 0,
                          "Pagespeed: cannot set gzip_http_version,"
                          " signature mismatch");
            signature_mismatch_2nd++;
          }
        }

        if (!gzip_proxied_command_.command_ &&
            ngx_strcmp("gzip_proxied", current_command->name.data) == 0) {
          if (IsNgxBitmaskCommand(current_command)) {
            current_command->set = ngx_gzip_redirect_conf_set_bitmask_slot;
            gzip_proxied_command_.command_ = current_command;
            gzip_proxied_command_.module_ = ngx_modules[m];
          } else {
            ngx_log_error(NGX_LOG_WARN, ngx_cycle->log, 0,
                          "Pagespeed: cannot set gzip_proxied,"
                          " signature mismatch");
            signature_mismatch_2nd++;
          }
        }

        if (!gzip_http_types_command_.command_ &&
            ngx_strcmp("gzip_types", current_command->name.data) == 0) {
          if (IsNgxHttpTypesCommand(current_command)) {
            current_command->set = ngx_gzip_redirect_http_types_slot;
            gzip_http_types_command_.command_ = current_command;
            gzip_http_types_command_.module_ = ngx_modules[m];
          } else {
            ngx_log_error(NGX_LOG_WARN, ngx_cycle->log, 0,
                          "Pagespeed: cannot set gzip_types,"
                          " signature mismatch");
            signature_mismatch_2nd++;
          }
        }

        if (!gzip_vary_command_.command_ &&
            ngx_strcmp("gzip_vary", current_command->name.data) == 0) {
          if (IsNgxFlagCommand(current_command)) {
          current_command->set = ngx_gzip_redirect_conf_set_flag_slot;
          gzip_vary_command_.command_ = current_command;
          gzip_vary_command_.module_ = ngx_modules[m];
          } else {
            ngx_log_error(NGX_LOG_WARN, ngx_cycle->log, 0,
                          "Pagespeed: cannot set gzip_vary,"
                          " signature mismatch");
            signature_mismatch_2nd++;
          }
        }
      }
    }
  }
  if (signature_mismatch_1st) {
    return kInitGZipSignatureMismatchFatal;
  } else if (!enabled_) {
    ngx_log_error(NGX_LOG_WARN, ngx_cycle->log, 0,
                  "Pagespeed: cannot set gzip, command not found");
    return kInitGZipNotFound;
  } else if (signature_mismatch_2nd) {
    return kInitGZipSecondarySignatureMismatch;
  } else if (!gzip_vary_command_.command_) {
    ngx_log_error(NGX_LOG_WARN, ngx_cycle->log, 0,
                  "Pagespeed: missing gzip_vary");
    return kInitGZipSecondaryMissing;
  } else if (!gzip_http_types_command_.command_) {
    ngx_log_error(NGX_LOG_WARN, ngx_cycle->log, 0,
                  "Pagespeed: missing gzip_types");
    return kInitGZipSecondaryMissing;
  } else if (!gzip_http_version_command_.command_) {
    ngx_log_error(NGX_LOG_WARN, ngx_cycle->log, 0,
                  "Pagespeed: missing gzip_http_version");
    return kInitGZipSecondaryMissing;
  } else if (!gzip_proxied_command_.command_) {
    ngx_log_error(NGX_LOG_WARN, ngx_cycle->log, 0,
                  "Pagespeed: missing gzip_proxied");
    return kInitGZipSecondaryMissing;
  } else {
    return kInitGZipOk;
  }
}

void* ngx_command_ctx::GetConfPtr(ngx_conf_t* cf) {
  char* conf_ptr = reinterpret_cast<char*>(GetModuleConfPtr(cf));
  return (conf_ptr+command_->offset);
}

void* ngx_command_ctx::GetModuleConfPtr(ngx_conf_t* cf) {
  return (ngx_http_conf_get_module_loc_conf(cf, (*(module_))));
}

void NgxGZipSetter::SetNgxConfFlag(
    ngx_conf_t* cf,
    ngx_command_ctx* command_ctx,
    ngx_flag_t value
    ) {
  ngx_flag_t* flag =
      reinterpret_cast<ngx_flag_t*>(command_ctx->GetConfPtr(cf));
  *flag = value;
  // Save the flag position for possible rollback.
  ngx_flags_set_.push_back(flag);
}

void NgxGZipSetter::SetNgxConfEnum(
    ngx_conf_t* cf,
    ngx_command_ctx* command_ctx,
    ngx_uint_t value
    ) {
  ngx_uint_t* enum_to_set =
      reinterpret_cast<ngx_uint_t*>(command_ctx->GetConfPtr(cf));
  *enum_to_set = value;
  ngx_uint_set_.push_back(enum_to_set);
}

void NgxGZipSetter::SetNgxConfBitmask(
    ngx_conf_t* cf,
    ngx_command_ctx* command_ctx,
    ngx_uint_t value
    ) {
  ngx_uint_t* enum_to_set =
      reinterpret_cast<ngx_uint_t*>(command_ctx->GetConfPtr(cf));
  *enum_to_set = value;
  ngx_uint_set_.push_back(enum_to_set);
}


// These are the content types we want to compress.
ngx_str_t gzip_http_types[] = {
    ngx_string("application/ecmascript"),
    ngx_string("application/javascript"),
    ngx_string("application/json"),
    ngx_string("application/pdf"),
    ngx_string("application/postscript"),
    ngx_string("application/x-javascript"),
    ngx_string("image/svg+xml"),
    ngx_string("text/css"),
    ngx_string("text/csv"),
    // ngx_string("text/html"),  // this is the default implied value
    ngx_string("text/javascript"),
    ngx_string("text/plain"),
    ngx_string("text/xml"),
    ngx_null_string  // indicates end of array
};

gzs_enable_result NgxGZipSetter::SetGZipForLocation(
    ngx_conf_t* cf,
    ngx_flag_t value) {
  if (!enabled_)
      return kEnableGZipNotEnabled;
  if (gzip_command_.command_) {
      SetNgxConfFlag(cf, &gzip_command_, value);
  }
  return kEnableGZipOk;
}

//@keesspoelstra: return value seems to be ignored
gzs_enable_result NgxGZipSetter::EnableGZipForLocation(ngx_conf_t* cf) {
  if (!enabled_) {
    return kEnableGZipNotEnabled;
  }

  // When we get called twice for the same location{}, we ignore the second call
  // to prevent adding duplicate gzip http types and so on.
  ngx_flag_t* flag =
      reinterpret_cast<ngx_flag_t*>(gzip_command_.GetConfPtr(cf));
  if (*flag == 1) {
    return kEnableGZipOk;
  }
  SetGZipForLocation(cf, 1);
  if (gzip_vary_command_.command_) {
    SetNgxConfFlag(cf, &gzip_vary_command_, 1);
  }
  if (gzip_http_version_command_.command_) {
    SetNgxConfEnum(cf, &gzip_http_version_command_, NGX_HTTP_VERSION_10);
  }
  if (gzip_proxied_command_.command_) {
      SetNgxConfBitmask(cf,
                        &gzip_http_version_command_,
                        NGX_HTTP_GZIP_PROXIED_ANY);
  }

  // @keesspoelstra: can we remove this comment? Or
  // does this need more work?
  // Do we want to do this, this is actually
  // prone to future API changes, the signature
  // check should be enough to prevent problems.
  AddGZipHTTPTypes(cf);
  return kEnableGZipOk;
}

// @keesspoelstra: either remove the todo or fix it?
// TODO(kspoelstra): return status.
void NgxGZipSetter::AddGZipHTTPTypes(ngx_conf_t* cf) {
  if (gzip_http_types_command_.command_) {
    // Following should not happen, but if it does return gracefully.
    if (cf->args->nalloc < 2) {
      ngx_log_error(NGX_LOG_WARN, ngx_cycle->log, 0,
                    "Pagespeed: unexpected small cf->args in gzip_types ");
      return;
    }

    ngx_command_t* command = gzip_http_types_command_.command_;
    char* gzip_conf = reinterpret_cast<char* >(
        gzip_http_types_command_.GetModuleConfPtr(cf));

    // Backup the old settings.
    ngx_str_t old_elt0 = reinterpret_cast<ngx_str_t*>(cf->args->elts)[0];
    ngx_str_t old_elt1 = reinterpret_cast<ngx_str_t*>(cf->args->elts)[1];
    ngx_uint_t old_nelts = cf->args->nelts;

    // Setup first arg.
    ngx_str_t gzip_types_string = ngx_string("gzip_types");
    reinterpret_cast<ngx_str_t*>(cf->args->elts)[0]=gzip_types_string;
    cf->args->nelts = 2;

    ngx_str_t* http_types = gzip_http_types;
    while (http_types->data) {
      ngx_str_t d;
      // TODO(kspoelstra): We allocate the http type on the configuration pool and actually
      // leak this if we rollback. We cannot use the gzip_http_types array here
      // because nginx will manipulate the values.
      // TODO(kspoelstra): better would be to allocate this once and not every time we
      // enable gzip.
      d.data = reinterpret_cast<u_char* >(
          ngx_pnalloc(cf->pool, http_types->len+1));
      snprintf(reinterpret_cast<char* >(d.data), http_types->len + 1, "%s",
          reinterpret_cast<const char* >(http_types->data));
      d.len = http_types->len;
      reinterpret_cast<ngx_str_t*>(cf->args->elts)[1] = d;
      // Call the original setter.
      ngx_http_types_slot(cf, command, gzip_conf);
      http_types++;
    }

    // Restore args.
    cf->args->nelts = old_nelts;
    reinterpret_cast<ngx_str_t*>(cf->args->elts)[1] = old_elt1;
    reinterpret_cast<ngx_str_t*>(cf->args->elts)[0] = old_elt0;

    // Backup configuration location for rollback.
    ngx_httptypes_set_.push_back(gzip_conf+command->offset);
  }
}

void NgxGZipSetter::RollBackAndDisable() {
  ngx_log_error(NGX_LOG_INFO, ngx_cycle->log, 0,
                "Pagespeed: rollback gzip, explicit configuration");
  for (vector<ngx_flag_t*>::iterator i = ngx_flags_set_.begin();
      i != ngx_flags_set_.end();
      i++) {
    *(*i)=NGX_CONF_UNSET;
  }
  for (vector<ngx_uint_t*>::iterator i = ngx_uint_set_.begin();
        i != ngx_uint_set_.end();
        i++) {
      *(*i)=NGX_CONF_UNSET_UINT;
  }
  for (vector<void*>::iterator i = ngx_httptypes_set_.begin();
      i != ngx_httptypes_set_.end();
      i++) {
    ngx_array_t** type_array = reinterpret_cast<ngx_array_t**> (*i);
    ngx_array_destroy(*type_array);

    *type_array = NULL;
  }
  enabled_ = 0;
}

}  // namespace net_instaweb
