#include <math/linear_math.h>

using namespace std;

void testfunc() {
    Vec3f myVec = Vec3f(1.0f, 2.0f, 3.0f);
    Vec3f myVec2 = Vec3f(Vec3f(1.0f, 2.0f, 3.0f)[0], 5, 6);
    Vec3f temp = Vec3f(1.0f, 1.0f, 1.0f);
    temp += 1.0f;
    Vec3f myVec3 = 3.0f * temp * 10.0f;
    cout << "myVec = " << myVec << endl;
    cout << "myVec2 = " << myVec2 << endl;
    cout << "myVec3 = " << myVec3 << endl;
}
