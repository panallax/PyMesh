FROM python:3.10-bookworm
WORKDIR /root
ARG BRANCH="main"
ARG NUM_CORES=2

RUN apt-get update && apt-get install -y \
    gcc-9 \
    g++-9 \
    git \
    cmake \
    libgmp-dev \
    libmpfr-dev \
    libgmpxx4ldbl \
    libboost-dev \
    libboost-thread-dev \
    libc6\
    zip unzip patchelf && \
    apt-get clean

RUN git clone --single-branch -b $BRANCH https://github.com/panallax/PyMesh.git

ENV PYMESH_PATH /root/PyMesh
ENV NUM_CORES $NUM_CORES
WORKDIR $PYMESH_PATH

RUN git submodule update --init && \
    pip install -r $PYMESH_PATH/python/requirements.txt && \
    ./setup.py bdist_wheel && \
    rm -rf build_3.7 third_party/build && \
    python $PYMESH_PATH/docker/patches/patch_wheel.py dist/pymesh2*.whl && \
    pip install --upgrade pip && \
    pip install dist/pymesh2*.whl && \
    python -c "import pymesh; pymesh.test()"


CMD ["sleep", "infinity"]
