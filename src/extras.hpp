#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078

#define deg(x) (((x)*180.0)/PI)
#define rad(x) (((x)*PI)/180)
#define cot(x) tan((PI/2)-(x))

double linear_map(const double x,
                  const double min,
                  const double max,
                  const double new_min, const double new_max)
{
    return (((x - min) * (new_max - new_min)) / (max - min)) + new_min;
}

double percentage(double x, double min, double max)
{
    return linear_map(x, min, max, 0, 100);
}
