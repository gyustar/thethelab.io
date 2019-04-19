package com.ham;

public class WorkerThread extends Thread {
    @Override
    public void run() {
        int i;
        int local = 0;
        for (i = 0; i < 1000000000 / ThreadExample.getThreadCount(); ++i) {
            ++local;
        }

        synchronized (ThreadExample.getMutex()) {
            ThreadExample.setSum(ThreadExample.getSum() + local);
        }
    }
}
