## Installation

```sh
python3 -m venv .venv
source .venv/bin/activate
pip install lit
```

## Build

```sh
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON
cd build
make -j $(nproc --all)
clang-tidy -load=./libCodingStyleCheck.so -checks='readability-member-order' ../MemberOrderCheck.cpp
lit ../tests/ -vv
```
