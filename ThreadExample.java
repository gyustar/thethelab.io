class ThreadExample{
	private static long sum = 0;
	private static int threadCount = 1;

	public static void main(String[] args) {
		if(args.length == 1) {
			threadCount = Integer.parseInt(args[0]);
		}

		long start = System.currentTimeMillis();
	  System.out.println("스레드 개수 : " + threadCount);

		Thread[] threads = new Thread[threadCount];
		int i = 0;
		for (i = 0; i < threadCount; ++i) {
			threads[i] = new Thread() {
				public void run() {
					int j = 0;
					int local = 0;
					for (j = 0; j < 2000000000 / threadCount; ++j) {
						++local;
					}
					lock(local);
				}
			};
			threads[i].start();
		}
		for (i = 0; i < threadCount; ++i) {
			try {
				threads[i].join();
			} catch (InterruptedException e) {
				System.out.println("InterruptedException 발생");
			}
		}
		long end = System.currentTimeMillis();
		System.out.println(sum);
		System.out.println("실행 시간 : " + (end - start) / 1000.0);
	}

	public static synchronized void lock(int local) {
		sum += local;
	}
}

