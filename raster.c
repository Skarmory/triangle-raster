#include <math.h>
#include <stdio.h>

#define SIZE 128

struct vec
{
    float x, y;
};

float dot(struct vec* p1, struct vec* p2)
{
    return (p1->x * p2->x + p1->y * p2->y);
}

float length(struct vec* v)
{
    return sqrt(v->x * v->x + v->y * v->y);
}

void normalise(struct vec* v)
{
    float l = length(v);
    v->x /= l;
    v->y /= l;
}

int main(void)
{
    // Triangle points
    struct vec a = { 10.f, 10.f };
    struct vec b = { 20.f, 100.f };
    struct vec c = { 60.f, 40.f };

    // Only need A-B because we can simply multiply the projection of
    // A-B onto n1 by -1, which has the same effect as projecting B-A onto n1
    struct vec ab = { a.x - b.x, a.y - b.y };

    // These are the normals of sides BC and AC
    // Only need the two normals because we don't need to directly calculate gamma
    // As gamma = 1 - alpha - beta
    struct vec n1 = { 60.f, 40.f };
    struct vec n2 = { 30.f, -50.f };

    // Normalise them to avoid sqrting every time to get their magnitude
    normalise(&n1);
    normalise(&n2);

    // Write directly into a P3 ppm file
    FILE* f = fopen("triangle.ppm", "w");
    fprintf(f, "P3\n%d %d\n255\n", SIZE, SIZE);

    // Write for loop this way because we need to write the file from top to bottom
    // So we need to start with the top of the image. y is image rows, x is image cols
    for(int y = SIZE-1; y >= 0; --y)
    {
        for(int x = 0; x < SIZE; ++x)
        {
            // 1) Dot the vector from P to whichever corner we need with the normal
            //    of the line which that corner is a point of to get the distance.
            //
            // 2) Do the same for the corresponding vector made from minusing the
            //    two relevant corner vectors
            //
            // 3) Divide the distance of P by the distance of the relevant corner point
            //
            // Don't need to divide because the normals were normalised earlier

            float alpha = 0.f, beta = 0.f, gamma = 0.f;
            float num = 0.f, denom = 1.f;

            struct vec p = { (float)x, (float)y };
            struct vec ap = { a.x - p.x, a.y - p.y };
            struct vec bp = { b.x - p.x, b.y - p.y };

            // Alpha
            num = dot(&bp, &n1);
            denom = dot(&ab, &n1) * -1.f;
            alpha = num / denom;

            // Beta
            num = dot(&ap, &n2);
            denom = dot(&ab, &n2);
            beta = num / denom;

            // Gamma
            gamma = 1.f - alpha - beta;

            // Check if they're in the triangle and set a colour value
            // Alpha is r, beta is g, and gamma is b
            if(alpha+beta+gamma <= 1.f && alpha >= 0.f && beta >= 0.f && gamma >= 0.f)
                fprintf(f, "%d %d %d ", (unsigned char)ceil(255.f * alpha), (unsigned char)ceil(255.f * beta), (unsigned char)ceil(255.f * gamma));
            else
                fprintf(f, "%d %d %d ", 0,0,0);
        }

        fprintf(f, "\n");
    }

    fclose(f);

    return 0;
}
