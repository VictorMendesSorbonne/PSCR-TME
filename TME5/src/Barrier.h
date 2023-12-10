#pragma once

namespace pr {
    class Barrier {

        int cpt;
        std::mutex m;
        int N;
        std::condition_variable cv;

    public :
        Barrier(int N) : N(N) {
            cpt = 0;
        }

        void done() {
            std::unique_lock <std::mutex> l(m);
            cpt++;
            if (N >= cpt) {
                cv.notify_one();
            }

        }

        void wait_for() {
            std::unique_lock <std::mutex> l(m);
            while (N < cpt) {           //If a signal wake the thread and not the thread which notify him
                cv.wait(l);
            }
        }
    };
}