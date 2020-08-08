FROM moku828/gcc-sh-elf:7.4.0
RUN mkdir /build \
&& cd /build \
&& git clone https://bitbucket.org/moku828/qemu.git \
&& cd qemu \
&& git checkout develop/2020.08_v5.0.0 \
&& ./configure --target-list=sh4eb-softmmu --enable-debug-tcg --cross-cc-sh4eb=sh-elf-gcc \
&& make -s -j8 \
&& make -s install \
&& cd / \
&& rm -rf build
