package com.ham;

public class ThreadExample {
    private static int sum = 0;
    private static int threadCount = 1;
    private static final Object MUTEX = new Object();

    static int getSum() {
        return sum;
    }

    static void setSum(int sum) {
        if(sum > 0) {
            ThreadExample.sum = sum;
        }
    }

    static int getThreadCount() {
        return threadCount;
    }

    static Object getMutex() {
        return MUTEX;
    }

    public static void main(String[] args) {
        if (args.length >= 1) {
            threadCount = Integer.parseInt(args[0]);
        }

        long start = System.currentTimeMillis();
        Thread[] threads = new Thread[threadCount];

        for (int i = 0; i < threadCount; ++i) {
            threads[i] = new WorkerThread();
            threads[i].start();
        }

        for (int i = 0; i < threadCount; ++i) {
            try {
                threads[i].join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        long end = System.currentTimeMillis();
        System.out.println(sum);
        System.out.println("실행 시간 : " + (end - start) / 1000.0);
    }
}
