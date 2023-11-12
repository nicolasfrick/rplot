#include "rplot.h"

using namespace rplot;

int main()
{
  MainFrame mf1;
  MainFrame mf2;
  std::vector<double> x;

  for (size_t idx = 0; idx < 100; idx++)
  {
    mf1.addData1(idx, sin(idx*0.1), 2*cos(idx*0.2), 0.5*cos(idx*3));
    mf1.addData2(idx, sin(idx*0.5), 0.5*cos(idx*0.1), 2.5*cos(idx*0.3));
    mf1.addError1(idx*1e-3, 1, 0.5*cos(idx*0.1)*1e-1, 2.5*cos(idx*0.3)*1e-3);
    x.push_back(idx);
    mf2.addData1(idx, sin(idx*0.1), 2*cos(idx*0.2), 0.5*cos(idx*3));
  }

  mf1.savePdf(true, true, true, false, "error_plot.pdf", true, true, "Error plot");
  mf2.savePdfHisto(x, "histo_plot.pdf");
  mf2.plot(true, true, true);

  return 0;
}
