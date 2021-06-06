# lithium-realworld-example-app

Lithium c++ framework real world example

* [Reaworld example](https://github.com/gothinkster/realworld)
* [Lithium](https://github.com/matt-42/lithium)

## Requirements

Lithium shoud be installed locally:
[Install](https://github.com/matt-42/lithium/blob/master/INSTALL.md)

## Usage

Run docker postgres database container:

```bash
docker run --rm -d \
    --name lithium-realworld-example-db \
    --network host \
    -e POSTGRES_PASSWORD=lithium_test \
    postgres
```

Build app:

```bash
mkdir -v build
cd build
cmake ..
make
```
Run app:

```bash
./lithium-realworld-example-app
```

Execute tests:

```bash
./test/newman.sh
```

Stop database container:

```bash
docker stop lithium-realworld-example-db
```