#include <TApplication.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TArrow.h>
#include <TLatex.h>
#include <TGClient.h>
#include <TF1.h>
#include <TRandom.h>
#include <TGButton.h>
#include <TCanvas.h>
#include <TMultiGraph.h>
#include <ros/package.h>

#include "../include/rplot.h"

using namespace rplot;

MainFrame::MainFrame()
  :cnt(0),
   fMain(nullptr)
{
  ROOT::EnableThreadSafety();
}

void MainFrame::addData1(const double t, const double pos, const double vel, const double acc)
{
  pos1.push_back(pos);
  vel1.push_back(vel);
  double jerk = 0.0;
  if (acc1.size() > 0 && t > 0.0)
    jerk = (acc - *acc1.end()) / (t - *t1.end());
  acc1.push_back(acc);
  jerk1.push_back(jerk);
  t1.push_back(t);
}

void MainFrame::clearData1()
{
  pos1.clear();
  vel1.clear();
  acc1.clear();
  jerk1.clear();
  t1.clear();
}

void MainFrame::plot(const bool plt_pos1, const bool plt_vel1, const bool plt_acc1)
{
  if (t1.size() == 0)
    return;

  int argc = 0;
  char arg[] = "";
  char *argv[1] = {arg};
  // create system interface
  TApplication app("App",&argc,argv);
  // create window
  createMainFrame(gClient->GetRoot(),400,400);
  // draw graph
  doDraw(plt_pos1, plt_vel1, plt_acc1);
  // empty memory
  clearData1();
  // keep window open
  app.Run();
}

void MainFrame::createMainFrame(const TGWindow *p,UInt_t w,UInt_t h)
{
   // Create a main frame
   fMain = new TGMainFrame(p,w,h);

   // Create canvas widget
   fEcanvas = new TRootEmbeddedCanvas("Ecanvas",fMain,200,200);
   fMain->AddFrame(fEcanvas, new TGLayoutHints(kLHintsExpandX |
                   kLHintsExpandY, 10,10,10,1));

   // Create a horizontal frame widget with buttons
   TGHorizontalFrame *hframe = new TGHorizontalFrame(fMain,200,40);
   TGTextButton *exit = new TGTextButton(hframe,"&Exit",
                                "gApplication->Terminate(0)");
   hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX,
                                            5,5,3,4));
   fMain->AddFrame(hframe, new TGLayoutHints(kLHintsCenterX,
                                             2,2,2,2));

   // Set a name to the main frame
   fMain->SetWindowName("Plot");

   // Map all subwindows of main frame
   fMain->MapSubwindows();

   // Initialize the layout algorithm
   fMain->Resize(fMain->GetDefaultSize());

   // Map main frame
   fMain->MapWindow();
}

MainFrame::~MainFrame()
{
  // Clean up used widgets: frames, buttons, layout hints
  if (fMain != nullptr)
  {
    fMain->Cleanup();
    delete fMain;
  }
}

void MainFrame::doDraw(const bool plt_pos1, const bool plt_vel1, const bool plt_acc1)
{
  TMultiGraph mg;
  TGraph *g1, *g2, *g3;
  TLegend leg(.1,.7,.3,.9,"");
  leg.SetFillColor(0);

  if (plt_pos1)
  {
    g1 = new TGraph(static_cast<Int_t>(t1.size()), t1.data(), pos1.data());
    g1->SetLineColor(kBlue);
    g1->SetLineStyle(2);
    g1->SetLineWidth(1);
    mg.Add(g1);
    leg.AddEntry(g1,"Position");
  }

  if (plt_vel1)
  {
    g2 = new TGraph(static_cast<Int_t>(t1.size()), t1.data(), vel1.data());
    g2->SetLineColor(kYellow);
    g2->SetLineStyle(2);
    g2->SetLineWidth(1);
    mg.Add(g2);
    leg.AddEntry(g2,"Velocity");
  }

  if (plt_acc1)
  {
    g3 = new TGraph(static_cast<Int_t>(t1.size()), t1.data(), acc1.data());
    g3->SetLineColor(kRed);
    g3->SetLineStyle(2);
    g3->SetLineWidth(1);
    mg.Add(g3);
    leg.AddEntry(g3,"Acceleration");
  }

  mg.SetTitle("Plot");
  mg.DrawClone("LA");

  leg.DrawClone("Same");

  TCanvas *fCanvas = fEcanvas->GetCanvas();
  fCanvas->SetGrid();
  fCanvas->cd();
  fCanvas->Update();
}

void MainFrame::savePdf(const bool plt_pos1, const bool plt_vel1, const bool plt_acc1, const bool plt_jerk1, const std::string save_file)
{
  if (t1.size() == 0 || cnt > 100)
  {
    return;
  }

  TCanvas canvas("Canvas");
  TMultiGraph mg;
  TGraph *g1=nullptr, *g2=nullptr, *g3=nullptr, *g4=nullptr;
  TLegend leg(.1,.7,.3,.9,"");
  leg.SetFillColor(0);

  if (plt_pos1 && pos1.size())
  {
    g1 = new TGraph(static_cast<Int_t>(t1.size()), t1.data(), pos1.data());
    g1->SetLineColor(kBlue);
    mg.Add(g1);
    leg.AddEntry(g1,"Position");
  }

  if (plt_vel1 && vel1.size())
  {
    g2 = new TGraph(static_cast<Int_t>(t1.size()), t1.data(), vel1.data());
    g2->SetLineColor(kGreen);
    mg.Add(g2);
    auto maxit = std::minmax_element(vel1.begin(), vel1.end());
    auto absmax = {std::fabs(*maxit.first), std::fabs(*maxit.second)};
    std::string txt = "Velocity " + std::to_string(*std::max_element(absmax.begin(), absmax.end())) + " rad/s";
    leg.AddEntry(g2,txt.c_str());
  }

  if (plt_acc1 && acc1.size())
  {
    g3 = new TGraph(static_cast<Int_t>(t1.size()), t1.data(), acc1.data());
    g3->SetLineColor(kRed);
    mg.Add(g3);
    auto maxit = std::minmax_element(acc1.begin(), acc1.end());
    auto absmax = {std::fabs(*maxit.first), std::fabs(*maxit.second)};
    std::string txt = "Acceleration " + std::to_string(*std::max_element(absmax.begin(), absmax.end())) + " rad/s^2";
    leg.AddEntry(g3,txt.c_str());
  }

  if (plt_jerk1 && jerk1.size())
  {
    g4 = new TGraph(static_cast<Int_t>(t1.size()), t1.data(), jerk1.data());
    g4->SetLineColor(kViolet);
    mg.Add(g4);
    auto maxit = std::minmax_element(jerk1.begin(), jerk1.end());
    auto absmax = {std::fabs(*maxit.first), std::fabs(*maxit.second)};
    std::string txt = "Jerk " + std::to_string(*std::max_element(absmax.begin(), absmax.end())) + " rad/s^3";
    leg.AddEntry(g4,txt.c_str());
  }

  mg.SetTitle("Plot");
  mg.Draw("LA");

  leg.Draw("Same");

  canvas.SetGrid();
  std::string fn = ros::package::getPath("rplot") + "/out/" + std::to_string(cnt++) + "_" + save_file;
  canvas.Print(fn.c_str());

  clearData1();
}
