FROM alpine:3.19 AS build

RUN apk add --no-cache \
    g++ \
    cmake \
    make \
    wt-dev

WORKDIR /app

COPY CMakeLists.txt .
COPY include ./include
COPY src ./src
COPY content ./content

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
 && cmake --build build --config Release

# ---------- Runtime ----------
FROM alpine:3.19 AS runtime

RUN apk add --no-cache wt

WORKDIR /app

# Binary
COPY --from=build /app/build/portfolio_wt /app/portfolio_wt

# Static files (CSS, favicon, robots.txt)
COPY public /app/public

# Content (JSON)
COPY content /app/content

EXPOSE 8080

CMD ["./portfolio_wt", "--docroot=public", "--http-address=0.0.0.0", "--http-port=8080"]
