
extern "C" {
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_http.h>
}

#include <vector>

using namespace std;


extern "C" {char *ngx_gzip_redirect_conf_set_flag_slot(ngx_conf_t *cf, ngx_command_t *cmd, void *conf);}



class NgxGzipSetter
{

  vector<void *> _confs;
  ngx_command_t *_gzip_command;
  ngx_module_t *_gzip_module;
  ngx_flag_t _enabled;
 public:
  NgxGzipSetter():_gzip_command(NULL),_gzip_module(NULL),_enabled(0)
  {

  }
  ~NgxGzipSetter()
  {
    ;
  }

  ngx_flag_t enabled()
  {
    return _enabled;
  }

  void Init()
  {
    for (int m = 0; ngx_modules[m]; m++)
    {
      if (ngx_modules[m]->commands)
        for (int c=0;ngx_modules[m]->commands[c].name.len;c++)
        {
          ngx_command_t *current_command=&ngx_modules[m]->commands[c];

          if (ngx_strcmp("gzip", ngx_modules[m]->commands[c].name.data) == 0)
          {
            if (current_command->set==ngx_conf_set_flag_slot &&
                        !(current_command->type&(NGX_DIRECT_CONF|NGX_MAIN_CONF)) &&
                        current_command->conf==NGX_HTTP_LOC_CONF_OFFSET)
            {
              _gzip_command=&ngx_modules[m]->commands[c];
              _gzip_command->set=ngx_gzip_redirect_conf_set_flag_slot;
              _gzip_module=ngx_modules[m];
              _enabled=1;
            }
            break;
          }

        }
    }
  }

  void SetGZip(ngx_conf_t *cf)
  {
    if (!_enabled)
      return;
    void *gzip_conf=ngx_http_conf_get_module_loc_conf(cf,(*_gzip_module));
    ngx_flag_t *flag=(ngx_flag_t *) (((char *) gzip_conf)+_gzip_command->offset);
    *flag=1;
    _confs.push_back(flag);
  }


  void RollBackAndDisable()
  {
    for(vector<void *>::iterator i=_confs.begin();i!=_confs.end();i++)
    {
      *((ngx_flag_t *) *i )=NGX_CONF_UNSET;
    }
    _enabled=0;
  }
};

extern NgxGzipSetter gzip_setter;
