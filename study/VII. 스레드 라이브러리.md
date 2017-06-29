# Thread
- LWP(Light Weight Process, 경량 프로세스)
- 자원 공유, 독립적인 스케줄링 단위
- 자원을 공유하기 때문에 스레드 간 동기화가 중요
- C 프로그램 컴파일 시 POSIX Thread 라이브러리를 링크해야 한다.
  - gcc -o hello hello.c -lpthread
  - lpthread : library posix thread
- mult-thread 프로그램에서 한 프로세스 안의 스레드들 간에는 특별한 처리 없이 동일한 메모리 주소 공간과 동일한 파일 디스크립터 접근 가능

# KLT [BSD, Solaris, Windows NT vs. Linux]

![alt text]({{ site.baseurl }}/image/lsp/VII/KLT.png)

- KLT(Kernel Level Thread), ULT(User Level Thread)
- Linux는 KLT를 지원하지 않음, Linux 이외의 대부분의 OS는 KLT 지원
