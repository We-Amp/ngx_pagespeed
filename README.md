# ngx_pagespeed

**ngx_pagespeed is maintained again — under the [ModPageSpeed](https://modpagespeed.com/) project at We-Amp.**

ngx_pagespeed was created at Google as the nginx port of mod_pagespeed. Google released its final upstream version in 2020. We-Amp picked it up. Active development continues under two product lines, both nginx-supported:

- **[ModPageSpeed 2.0](https://modpagespeed.com/)** — a ground-up C++23 rewrite. Available **today** as a Docker / nginx reverse-proxy and as an ASP.NET Core middleware. A bare-metal nginx module is in active development.
- **[mod_pagespeed 1.1](https://modpagespeed.com/1.1/)** — the maintained continuation of the open-source module. Drop-in replacement for the original mod_pagespeed and ngx_pagespeed configurations, with security patches and the new Cyclone Cache. **Apache and IIS are GA today; nginx ships next.**

If you are running ngx_pagespeed today, your existing configuration is compatible with both product lines.

| | |
|---|---|
| **Try ModPageSpeed 2.0 (Docker / nginx reverse proxy)** | [Quickstart →](https://modpagespeed.com/docs/) |
| **Run nginx today and want a drop-in upgrade** | [mod_pagespeed 1.1 →](https://modpagespeed.com/1.1/) |
| **Get notified when nginx 1.1 / nginx 2.0 ship** | [Sign up →](https://modpagespeed.com/download/) |
| **Pricing** | [$49/server/month — 14-day free trial →](https://modpagespeed.com/pricing/) |
| **Support** | [Email the maintainer →](https://modpagespeed.com/contact/) |

## What you get

- **Drop-in configuration** — your existing `pagespeed` directives keep working.
- **Security patches** for known CVEs that accumulated against the archived upstream.
- **Cyclone Cache** — a new C++23 lock-free shared-memory cache; replaces the legacy file cache. No tuning, automatic warm-up.
- **Active maintenance** — regular releases, modern Bazel build, pre-built binaries for amd64 and arm64.
- **Direct maintainer support** included with every license.

## About this repository

This repository exists as a public landing point for the ngx_pagespeed project under We-Amp's stewardship. Active development happens in the ModPageSpeed product repositories; **all downloads, documentation, and support are at [modpagespeed.com](https://modpagespeed.com/)**.

For issues or questions about a running deployment, please [contact the maintainer](https://modpagespeed.com/contact/).

## License

ModPageSpeed and mod_pagespeed 1.1 are distributed under the [Business Source License 1.1](https://modpagespeed.com/license/). The first 14 days are a free trial. See [pricing](https://modpagespeed.com/pricing/) for license details.

## Background

ngx_pagespeed was created at Google as the nginx port of the original mod_pagespeed Apache module — adopted by thousands of servers worldwide. After Google archived the project, We-Amp B.V. — a Dutch company founded by the former mod_pagespeed maintainer ([Otto van der Schaaf](https://github.com/oschaaf), Apache PageSpeed committer and IPMC member) — continued active development.

Learn more about We-Amp's open-source work: [we-amp.com/open-source/](https://we-amp.com/open-source/).
