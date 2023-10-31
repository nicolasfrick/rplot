#pragma once

#include <TROOT.h>
#include <TGFrame.h>
#include <TRootEmbeddedCanvas.h>
#include <TQObject.h>
#include <RQ_OBJECT.h>

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

      TGMainFrame         *fMain;
      TRootEmbeddedCanvas *fEcanvas;

      // draw graph
      void doDraw(const bool plt_pos1, const bool plt_vel1, const bool plt_acc1);
      // create window
      void createMainFrame(const TGWindow *p,UInt_t w,UInt_t h);

    public:
       MainFrame();
       virtual ~MainFrame();
       // create main app (system interface)
       void plot(const bool plt_pos1, const bool plt_vel1, const bool plt_acc1);
       // save to pdf w/o viewing
       void savePdf(const bool plt_pos1, const bool plt_vel1, const bool plt_acc1, const bool plt_jerk1, const std::string save_file);
       // add data for plotting
       void addData1(const double t, const double pos, const double vel=0.0, const double acc=0.0);
       void clearData1();
  };

}
