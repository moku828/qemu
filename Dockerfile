FROM moku828/pexpect:4.8.0 AS src
COPY . /qemu
WORKDIR /qemu

FROM src AS configure-for-debug
RUN ./configure --target-list=sh4eb-softmmu --enable-debug-tcg --cross-cc-sh4eb=sh-elf-gcc --static --disable-strip

FROM configure-for-debug AS make-for-debug
RUN make -s -j$(nproc)

FROM make-for-debug AS check-tcg
RUN make check-tcg

FROM src AS configure-for-release
RUN ./configure --target-list=sh4eb-softmmu --static

FROM configure-for-release AS make-for-release
RUN make -s -j$(nproc)

FROM make-for-release AS install
RUN make -s install

FROM alpine AS runner
COPY --from=install /usr/local/bin/qemu-system-sh4eb /usr/local/bin/
ENTRYPOINT [ "qemu-system-sh4eb" ]
CMD [ "--version" ]
