#include <TApplication.h>
#include <TGraphErrors.h>
#include <TArrow.h>
#include <TLatex.h>
#include <TGClient.h>
#include <TF1.h>
#include <TRandom.h>
#include <TGButton.h>
#include <TCanvas.h>
#include <ros/package.h>

#include "../include/rplot.h"

using namespace rplot;

MainFrame::MainFrame()
  :cnt(0),
   fMain(nullptr)
{
  ROOT::EnableThreadSafety();
}

void MainFrame::addData1(const double t, const double pos, const double vel, const double acc, const double jerk, const bool add_jerk)
{
  pos1.push_back(pos);
  vel1.push_back(vel);
  if (add_jerk)
    jerk1.push_back(jerk);
  else
  {
    if (acc1.size() > 0 && t > 0.0)
      jerk1.push_back((acc - acc1.back()) / (t - t1.back()));
    else
      jerk1.push_back(0.0);
  }
  acc1.push_back(acc);
  t1.push_back(t);
}

void MainFrame::addData2(const double t, const double pos, const double vel, const double acc, const double jerk, const bool add_jerk)
{
  pos2.push_back(pos);
  vel2.push_back(vel);
  if (add_jerk)
    jerk2.push_back(jerk);
  else
  {
    if (acc2.size() > 0 && t > 0.0)
      jerk2.push_back((acc - acc2.back()) / (t - t2.back()));
    else
      jerk2.push_back(0.0);
  }
  acc2.push_back(acc);
  t2.push_back(t);
}

void MainFrame::clearData1()
{
  pos1.clear();
  vel1.clear();
  acc1.clear();
  jerk1.clear();
  t1.clear();
}

void MainFrame::clearData2()
{
  pos2.clear();
  vel2.clear();
  acc2.clear();
  jerk2.clear();
  t2.clear();
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

void MainFrame::drawMultiGraph(TMultiGraph *mg, TGraph *g, TLegend *leg, std::vector<double> &data1, std::vector<double> &data2, EColor color, std::string label)
{
  g = new TGraph(static_cast<Int_t>(data1.size()), data1.data(), data2.data());
  g->SetLineColor(color);
  mg->Add(g);
  auto maxit = std::minmax_element(data2.begin(), data2.end());
  auto absmax = {std::fabs(*maxit.first), std::fabs(*maxit.second)};
  std::string txt = label + " " + std::to_string(*std::max_element(absmax.begin(), absmax.end()));
  leg->AddEntry(g, txt.c_str());
}

void MainFrame::savePdf(const bool plt_pos, const bool plt_vel, const bool plt_acc, const bool plt_jerk, const std::string save_file, const bool plt_data2)
{
  if (t1.size() == 0 || cnt > 100)
  {
    return;
  }

  TCanvas canvas("Canvas");
  if (plt_data2)
  {
    canvas.Divide(1,2);
    canvas.cd(1);
  }

  TMultiGraph mg1, mg2;
  TGraph *g11=nullptr, *g12=nullptr, *g13=nullptr, *g14=nullptr;
  TGraph *g21=nullptr, *g22=nullptr, *g23=nullptr, *g24=nullptr;
  TLegend leg1(.1,.7,.3,.9,"");
  leg1.SetFillColor(0);
  TLegend leg2 = leg1;

  if (plt_pos && pos1.size())
    drawMultiGraph(&mg1, g11, &leg1, t1, pos1, kBlue, "Position");
  if (plt_vel && vel1.size())
    drawMultiGraph(&mg1, g12, &leg1, t1, vel1, kGreen, "Velocity");
  if (plt_acc && acc1.size())
    drawMultiGraph(&mg1, g13, &leg1, t1, acc1, kRed, "Acceleration");
  if (plt_jerk && jerk1.size())
    drawMultiGraph(&mg1, g14, &leg1, t1, jerk1, kViolet, "Jerk");
  mg1.SetTitle("Plot1");
  mg1.Draw("LA");
  leg1.Draw("Same");

  if (plt_data2)
  {
    canvas.cd(2);
    if (plt_pos && pos2.size())
      drawMultiGraph(&mg2, g21, &leg2, t2, pos2, kBlue, "Position");
    if (plt_vel && vel2.size())
      drawMultiGraph(&mg2, g22, &leg2, t2, vel2, kGreen, "Velocity");
    if (plt_acc && acc2.size())
      drawMultiGraph(&mg2, g23, &leg2, t2, acc2, kRed, "Acceleration");
    if (plt_jerk && jerk2.size())
      drawMultiGraph(&mg2, g24, &leg2, t2, jerk2, kViolet, "Jerk");
    mg2.SetTitle("Plot2");
    mg2.Draw("LA");
    leg2.Draw("Same");
  }

  canvas.SetGrid();
  std::string fn = ros::package::getPath("rplot") + "/out/" + std::to_string(cnt++) + "_" + save_file;
  canvas.Print(fn.c_str());

  clearData1();
  if (plt_data2)
    clearData2();
}
