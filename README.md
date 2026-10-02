# ngx_pagespeed

> **This repository is archived. ngx_pagespeed lives on as the nginx module of
> [mod_pagespeed 2.1](https://github.com/We-Amp/mod_pagespeed), which is open source
> under the Apache License 2.0.**
>
> Source: [`We-Amp/mod_pagespeed` → `pagespeed/nginx/`](https://github.com/We-Amp/mod_pagespeed/tree/master/pagespeed/nginx)
> · Install: [nginx guide](https://github.com/We-Amp/mod_pagespeed/blob/master/docs/install-nginx.md)
> · Issues: [We-Amp/mod_pagespeed/issues](https://github.com/We-Amp/mod_pagespeed/issues)

## Where to go

| | |
|---|---|
| **Read or build the nginx module source** | [`pagespeed/nginx/` in We-Amp/mod_pagespeed →](https://github.com/We-Amp/mod_pagespeed/tree/master/pagespeed/nginx) |
| **Install the prebuilt, signed module** (`nginx-module-pagespeed`, apt/dnf, amd64 + arm64) | [Install guide →](https://github.com/We-Amp/mod_pagespeed/blob/master/docs/install-nginx.md) |
| **Documentation** | [modpagespeed.com/docs →](https://modpagespeed.com/docs/) |
| **Report a bug or ask a question** | [Open an issue →](https://github.com/We-Amp/mod_pagespeed/issues) |
| **Commercial support** | [Contact →](https://modpagespeed.com/contact/) |

## Why this repository is archived

The nginx, Apache, IIS and Envoy integrations of PageSpeed now live together in one
source tree, [We-Amp/mod_pagespeed](https://github.com/We-Amp/mod_pagespeed), where they
share a single build, test suite and release train across the supported platform
matrix. That tree is the continuation of this code: existing `pagespeed` directives
keep working, and it carries the security and correctness fixes that this archived
tree does not. Do not build from this repository; build from
[We-Amp/mod_pagespeed](https://github.com/We-Amp/mod_pagespeed) instead.

What you get there:

- **Drop-in** — existing ngx_pagespeed and mod_pagespeed configurations are compatible.
- **mod_pagespeed 2.1** — the converged product line: the serving module, the
  `pagespeed-optimizer` daemon for in-place resource optimization, and
  [Cyclone Cache](https://github.com/We-Amp/cyclone-cache), a C++23 shared-memory cache
  that replaces the legacy file cache.
- **Open source** — Apache License 2.0. [Support is what's for sale.](https://modpagespeed.com/pricing/)
- **Prebuilt, signed packages** for Debian, Ubuntu and EL9 (amd64 + arm64), built
  against each distribution's stock nginx.

This repository stays online, read-only, so that existing links, forks and the
2014-era release tags keep resolving.

## Background

ngx_pagespeed was created at Google as the nginx port of the original mod_pagespeed Apache module — adopted by thousands of servers worldwide. We-Amp's contribution is on the record: [Google's 2013 launch announcement](https://developers.googleblog.com/en/speed-up-your-sites-with-pagespeed-for-nginx/) credited ngx_pagespeed to "developers from Google, Taobao, We-Amp, and many other individual volunteers," and the [Apache Incubator PageSpeed proposal](https://cwiki.apache.org/confluence/display/INCUBATOR/PageSpeedProposal) lists We-Amp B.V. as a founding committer organization. After Google archived the project, We-Amp B.V. continued active development.

Learn more about We-Amp's open-source work: [we-amp.com/open-source/](https://we-amp.com/open-source/).
