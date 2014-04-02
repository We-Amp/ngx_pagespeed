
#include "ngx_gzip_setter.h"
#include <ngx_conf_file.h>


NgxGzipSetter gzip_setter;





extern "C" {
  char * ngx_gzip_redirect_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf)
  {
    if (gzip_setter.enabled()) gzip_setter.RollBackAndDisable();
    char *ret=ngx_conf_set_flag_slot(cf,cmd,conf);
    return ret;
  }
}
