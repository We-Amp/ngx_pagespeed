# ngx_pagespeed

**ngx_pagespeed is maintained again — under the [ModPageSpeed](https://modpagespeed.com/) project at We-Amp.**

ngx_pagespeed was created at Google as the nginx port of mod_pagespeed. Google released its final upstream version in 2020. We-Amp picked it up. Active development continues under two product lines:

- **[ModPageSpeed 2.0](https://modpagespeed.com/)** — a ground-up C++23 rewrite. Available **today** as a Docker / nginx reverse-proxy and as an ASP.NET Core middleware. A bare-metal, in-process nginx module is in active development.
- **[mod_pagespeed 1.15](https://modpagespeed.com/1.1/)** — the maintained continuation of the open-source module (renumbered from 1.1; Google's final was 1.14.36.1). Drop-in replacement for the original mod_pagespeed and ngx_pagespeed configurations, with security patches and the new Cyclone Cache. **nginx, Apache, and IIS are GA today** — nginx ships as prebuilt, signed apt/yum packages (`nginx-module-pagespeed`) for Debian and Ubuntu. Envoy is experimental.

If you are running ngx_pagespeed today, your existing configuration is compatible with both product lines.

| | |
|---|---|
| **Try ModPageSpeed 2.0 (Docker / nginx reverse proxy)** | [Quickstart →](https://modpagespeed.com/docs/) |
| **Run nginx today and want a drop-in upgrade** | [mod_pagespeed 1.15 →](https://modpagespeed.com/1.1/) |
| **Get notified when the native, in-process ModPageSpeed 2.0 nginx module ships** | [Sign up →](https://modpagespeed.com/download/) |
| **Pricing** | [$49/server/month — free to install and evaluate, license required for production →](https://modpagespeed.com/pricing/) |
| **Support** | [Email the maintainer →](https://modpagespeed.com/contact/) |

## What you get

- **Drop-in configuration** — your existing `pagespeed` directives keep working.
- **Security patches** for known CVEs that accumulated against the archived upstream.
- **Cyclone Cache** — a new C++23 lock-free shared-memory cache; replaces the legacy file cache. No tuning, automatic warm-up.
- **Active maintenance** — regular releases, modern Bazel build, pre-built signed apt/yum packages for amd64 and arm64.
- **Direct maintainer support** included with every license.

## About this repository

This repository exists as a public landing point for the ngx_pagespeed project under We-Amp's stewardship. Active development happens in the ModPageSpeed product repositories; **all downloads, documentation, and support are at [modpagespeed.com](https://modpagespeed.com/)**.

For issues or questions about a running deployment, please [contact the maintainer](https://modpagespeed.com/contact/).

## License

ModPageSpeed and mod_pagespeed 1.15 are distributed under the [Business Source License 1.1](https://modpagespeed.com/license/); source publication is planned (no date committed). You can install and run the module unlicensed to evaluate it — it fully optimizes and simply adds an `X-PageSpeed-Warn: unlicensed` response header. A commercial license is required for production use. See [pricing](https://modpagespeed.com/pricing/) for license details.

## Background

ngx_pagespeed was created at Google as the nginx port of the original mod_pagespeed Apache module — adopted by thousands of servers worldwide. After Google archived the project, We-Amp B.V. — a Dutch company that built ngx_pagespeed and supported mod_pagespeed under contract to Google, led by [Otto van der Schaaf](https://github.com/oschaaf) (Apache PageSpeed committer and IPMC member, 360+ pull requests across the codebase) — continued active development.

Learn more about We-Amp's open-source work: [we-amp.com/open-source/](https://we-amp.com/open-source/).
