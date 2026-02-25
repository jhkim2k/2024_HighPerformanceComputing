# 🚀 High Performance Computing (2024 초고속컴퓨팅)

이 저장소는 **초고속컴퓨팅(병렬컴퓨팅)** 과목의 실습(Lab) 및 과제(HW) 코드를 기록한 프로젝트입니다. 
순차적(Sequential) 처리 방식의 한계를 극복하기 위해 **Shared Memory(IPC), OpenMP, MPI, CUDA** 등 다양한 병렬 처리 기법을 단계별로 학습했습니다. [cite_start]이를 통해 알고리즘의 실행 시간(Elapsed Time) 단축 및 연산 성능(GFLOPS) 향상을 이루어낸 최적화 과정을 담고 있습니다[cite: 185, 1060, 1068].

## 🛠 Tech Stack
* **Language:** C (66.5%), CUDA C (33.5%)
* **Parallel Computing API/Architecture:** * OpenMP (Shared Memory Multiprocessing)
  * MPI (Message Passing Interface - Distributed Memory)
  * CUDA (GPU Programming)
* [cite_start]**OS/Environment:** Linux (Ubuntu) / hpc.gwnu.ac.kr 서버 환경 [cite: 928]

---

## 📂 Repository Structure

* `hw00` ~ `hw10_2`: 과제 수행 코드 (알고리즘 구현 및 성능 측정)
* `lab1` ~ `lab10`: 병렬 처리 API 및 문법 숙지를 위한 기초 실습 코드
* `*.pdf`: 각 과제 및 실습에 대한 상세 요구사항 명세서
* `*.cu.pdf`: CUDA 커널 구현 세부 코드가 포함된 자료

---

## 💡 Key Implementations & Evolution

본 프로젝트는 동일한 문제(예: 사다리꼴 공식을 이용한 적분 면적 계산)를 다양한 병렬화 기법으로 점진적으로 개선하며 성능을 비교 분석했습니다.

### 1. 면적 계산 병렬화 (Trapezoidal Rule)의 진화
사다리꼴 공식을 이용한 함수 그래프 하단 면적 계산을 다음 단계로 발전시켰습니다.
* [cite_start]**Linux & C Basics (HW0):** 리눅스 환경 서버 로그인 및 기초 C 프로그래밍 환경 구축.
* [cite_start]**Sequential:** 단일 스레드 기반 기본 면적 계산 (Lab 1)[cite: 198].
* [cite_start]**IPC (Shared Memory):** Semaphore를 활용한 클라이언트-서버 구조의 공유 메모리 누적 연산 (HW 2, Lab 2)[cite: 243, 259].
* [cite_start]**OpenMP:** `#pragma omp parallel` 및 `reduction` 절을 활용한 멀티 스레드 병렬 연산 (HW 3)[cite: 279].
* [cite_start]**MPI:** `MPI_Send` / `MPI_Recv`를 통한 분산 메모리 환경의 Sub-area 누적 연산 (HW 4)[cite: 283].
* [cite_start]**CUDA:** GPU Grid/Block/Thread 계층을 활용한 대규모 병렬 면적 계산 및 성능 측정 (HW 10)[cite: 184, 185, 1022]. 

### 2. MPI (Message Passing Interface) 주요 구현
다중 노드 환경에서의 데이터 통신 및 동기화 기법을 구현했습니다.
* [cite_start]**P2P & Collective Communication:** `MPI_Bcast`, `MPI_Scatterv`, `MPI_Gatherv`, `MPI_Reduce` 구현 (HW 5, HW 6, Lab 5)[cite: 140, 342, 360].
* [cite_start]**Grouping & Datatypes:** 구조체, Vector, Indexed Type을 이용한 효율적인 2D 블록(Matrix) 송수신 (Lab 6, Lab 7)[cite: 476, 509, 543].
* [cite_start]**Communicator & Topology:** `MPI_Comm_split`을 이용한 그룹 분할 및 Cartesian Grid Topology 생성, Boundary data 교환 (HW 8, Lab 8)[cite: 627, 632].
* [cite_start]**Parallel Sorting:** Parallel LogN Gather 알고리즘을 이용한 분산 정렬 구현 (HW 7)[cite: 151, 152].

### 3. CUDA (GPU Programming) 주요 구현
수천 개의 GPU 코어를 활용한 연산 최적화를 수행했습니다.
* [cite_start]**Thread Hierarchy:** 1D/2D Indexing (Grid, Block, Thread) 메모리 매핑 설계 (Lab 9)[cite: 733, 763].
* [cite_start]**Matrix Operations:** 대규모 2D 행렬 덧셈(`matadd`) 커널 구현 (HW 9)[cite: 176].
* [cite_start]**Shared Memory & Parallel Reduction:** GPU의 Shared Memory(`sdata`, `smax`)와 `__syncthreads()`를 이용한 병렬 Reduction 알고리즘을 구현하여 글로벌 메모리 병목 현상 개선 및 최댓값 탐색 (Lab 10, max.cu, area.cu)[cite: 898, 953, 969, 1025, 1037].
* **Performance Profiling:** `cudaEvent`를 활용한 커널 실행 시간(ms) 정밀 측정 및 GFLOPS 수치 도출 (HW 10, area.cu)[cite: 1060, 1068].

---

## 🚀 How to Run (Compile Guide)

각 디렉토리 내 코드는 사용된 기술에 따라 아래의 명령어로 컴파일 및 실행할 수 있습니다.

**1. OpenMP**
```bash
gcc -fopenmp -o program program.c
./program
```

**2. MPI**
```bash
mpicc -o program program.c
mpiexec -n <number_of_processes> ./program
```
*(주의: 2D Grid Topology나 특정 분할 알고리즘의 경우 프로세스 수가 제곱수여야 합니다[cite: 635].)*

**3. CUDA**
```bash
nvcc -o program program.cu
./program <threads_X> <threads_Y>
```
