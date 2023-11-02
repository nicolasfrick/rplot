#pragma once

#include <vector>
#include <TROOT.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include <TQObject.h>
#include <RQ_OBJECT.h>
#include <TLegend.h>
#include <TMultiGraph.h>

class TGWindow;
class TGMainFrame;
class TRootEmbeddedCanvas;

namespace rplot
{

  class MainFrame
  {
    RQ_OBJECT("MainFrame")

    private:
      int                 cnt;
      std::vector<double> pos1;
      std::vector<double> vel1;
      std::vector<double> acc1;
      std::vector<double> jerk1;
      std::vector<double> t1;
      std::vector<double> pos2;
      std::vector<double> vel2;
      std::vector<double> acc2;
      std::vector<double> jerk2;
      std::vector<double> t2;

      TGMainFrame         *fMain;
      TRootEmbeddedCanvas *fEcanvas;

      // draw graph
      void doDraw(const bool plt_pos1, const bool plt_vel1, const bool plt_acc1);
      // create window
      void createMainFrame(const TGWindow *p,UInt_t w,UInt_t h);
      // draw on multigraph
      void drawMultiGraph(TMultiGraph *mg, TGraph *g, TLegend *leg, std::vector<double> &data1, std::vector<double> &data2, EColor color, std::string label);

    public:
       MainFrame();
       virtual ~MainFrame();
       // create main app (system interface)
       void plot(const bool plt_pos1, const bool plt_vel1, const bool plt_acc1);
       // save to pdf w/o viewing
       void savePdf(const bool plt_pos, const bool plt_vel, const bool plt_acc, const bool plt_jerk, const std::string save_file, const bool plt_data2=false);
       // add data for plotting
       void addData1(const double t, const double pos, const double vel=0.0, const double acc=0.0, const double jerk=0.0, const bool add_jerk=false);
       void addData2(const double t, const double pos, const double vel=0.0, const double acc=0.0, const double jerk=0.0, const bool add_jerk=false);
       void clearData1();
       void clearData2();
  };

}
