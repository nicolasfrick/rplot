#include "rplot.h"

using namespace rplot;

int main()
{
  MainFrame mf;

  for (size_t idx = 0; idx < 100; idx++)
    mf.addData1(idx, sin(idx*0.1), 2*cos(idx*0.2), 0.5*cos(idx*3));

//  mf.plot(true, true, true);
  mf.savePdf(true, true, true, false, "plot.pdf");

  return 0;
}
