FROM ubuntu:20.04

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y build-essential git clang-format cppcheck aspell colordiff valgrind vim

RUN groupadd -g 10001 ubuntu && useradd -m -u 10000 -g ubuntu ubuntu
WORKDIR /home/ubuntu

RUN echo '\
set encoding=utf-8\n\
\n\
function! FormatOnSave()\n\
  let l:save_cursor = getpos(".")\n\
  execute "%!clang-format"\n\
  call setpos(".", save_cursor)\n\
endfunction\n\
\n\
autocmd BufWritePre *.h,*.hpp,*.c,*.cc,*.cpp call FormatOnSave()\n' > .vimrc

COPY . lab0-c
RUN chown -R ubuntu:ubuntu /home/ubuntu

WORKDIR /home/ubuntu/lab0-c

USER ubuntu:ubuntu
