int max = 10;
int reduce(int x, int y) {
    int kernelid = 1;
    if (kernelid == 0)
        return x + y;
    if (kernelid == 1) {
        // x ^ y;
        int ret = 0;
        int i = 1;
        while (i < max) {
            if (x / i % 2 == y / i % 2)
                ret = ret * 2;
            else
                ret = ret * 2 + 1;
            i = i * 2;
        }
        return ret;
    }
    if (kernelid == 2)
        if (x > y)
            return x;
        else
            return y;
    if (kernelid == 3) {
        // return x | y;

        int ret = 0;
        int i = 1;
        while (i < max) {
            if (x / i % 2 == 1 || y / i % 2 == 1)
                ret = ret * 2 + 1;
            else
                ret = ret * 2;
            i = i * 2;
        }
        return ret;
    }
    if (kernelid == 4) {
        // return x & y;
        int ret = 0;
        int i = 1;
        while (i < max) {
            if (x / i % 2 == 1 && y / i % 2 == 1)
                ret = ret * 2 + 1;
            else
                ret = ret * 2;
            i = i * 2;
        }
        return ret;
    }
    return 0;
}
