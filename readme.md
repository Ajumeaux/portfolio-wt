# portfolio_wt

Personal portfolio written in **C++** using **[Wt](https://www.webtoolkit.eu/wt)** (WebToolkit), packaged and deployed with **Docker Compose**.  
Page content (FR/EN) is stored in **JSON** files and rendered server-side with Wt widgets.

## Overview

- Backend: **C++17** + Wt (`wt`, `wthttp`)
- Deployment: **Docker** (Alpine) + `docker compose`
- Reverse proxy: **Traefik** (labels included in `docker-compose.yml`)
- Content: `content/**/(fr|en).json`
- Static assets: `public/` (CSS, JS, favicon, robots.txt)

---

## Run (Production) — Docker Compose

### Requirements
- Docker + Docker Compose
- An external Docker network named `traefik` (if using Traefik as provided)

### Start
'''bash
docker compose up -d --build
'''

### Logs
'''bash
docker compose logs -f
'''

### Stop
'''bash
docker compose down
'''

---

## Traefik setup

The `portfolio` service exposes the app on internal port **8080** and Traefik routes traffic using:

- `Host(\`portfolio.ajumax.com\`)`
- TLS enabled (`traefik.http.routers.portfolio.tls=true`)
- Certificate resolver: `porkbun`

Key points from `docker-compose.yml`:
- Service name: `portfolio`
- Traefik target port: `8080`
- Docker network used by Traefik: `traefik` (external)

> If you don’t use Traefik, you can expose the port directly with `ports: - "8080:8080"`.

---

## Run locally (without Traefik)

### Option A — Docker only
Add this to the `portfolio` service in `docker-compose.yml`:

'''yaml
ports:
  - "8080:8080"
'''

Then:
'''bash
docker compose up -d --build
'''

Open:
- http://localhost:8080

### Option B — Local build (CMake)

#### Requirements
- A C++17 compiler
- CMake ≥ 3.16
- Wt development packages (libraries `wt` and `wthttp` + headers)

#### Build
'''bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
'''

#### Run
'''bash
./build/portfolio_wt \
  --docroot=public \
  --http-address=0.0.0.0 \
  --http-port=8080
'''

---

## How it works (routing + language)

### Default docroot
The entrypoint (`main.cpp`) ensures a default `--docroot public` is present if not provided, so static assets can be served correctly.

### Internal routing
`PortfolioApp` uses Wt internal paths and listens to changes via:

- `internalPathChanged().connect(...)`
- `handleInternalPath(internalPath())`

Routes currently implemented:

- `/` → Home (previews: About / Projects / Contact)
- `/about` → About page
- `/projects` → Project list
- `/projects/<slug>` → Project detail page
- `/contact` → Contact page

### Language detection / switching
Language (`lang_`) is selected in this order:

1. Query parameter: `?lang=fr` / `?lang=en`
2. Browser locale (`env.locale().name()`)
3. Fallback to `en`

The language switcher updates the URL by injecting/replacing `lang` in the query string and then redirects, keeping the current internal path.

---

## Project structure

'''text
.
├── CMakeLists.txt
├── Dockerfile
├── docker-compose.yml
├── include/         # C++ headers
├── src/             # C++ sources
├── public/          # Static assets (CSS/JS/favicon/robots)
└── content/         # JSON content (FR/EN)
'''

### Content / i18n
The `content/` folder contains one JSON file per page and per language, for example:
- `content/about/fr.json`
- `content/about/en.json`
- `content/projects/fr.json`
- `content/projects/en.json`

---

## Docker notes

This project uses a multi-stage build:
1. **build** stage installs `g++`, `cmake`, `make`, `wt-dev` and compiles the binary
2. **runtime** stage installs `wt`, copies the binary + `public/` + `content/`, then runs:

'''bash
./portfolio_wt --docroot=public --http-address=0.0.0.0 --http-port=8080
'''

---

## License

Do whatever you want with this code — just keep the copyright notice.
Released under the MIT License. See the `LICENSE` file.

