#include <TApplication.h>
#include <TArrow.h>
#include <TLatex.h>
#include <TGClient.h>
#include <TF1.h>
#include <TRandom.h>
#include <TGButton.h>
#include <TCanvas.h>
#include <TFrame.h>
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

void MainFrame::addError1(const double t, const double pos, const double vel, const double acc, const double jerk)
{
  pos_err1.push_back(pos);
  vel_err1.push_back(vel);
  acc_err1.push_back(acc);
  jerk_err1.push_back(jerk);
  t_err1.push_back(t);
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

void MainFrame::addData3(const double t, const double pos, const double vel, const double acc, const double jerk, const bool add_jerk)
{
  pos3.push_back(pos);
  vel3.push_back(vel);
  if (add_jerk)
    jerk3.push_back(jerk);
  else
  {
    if (acc3.size() > 0 && t > 0.0)
      jerk3.push_back((acc - acc3.back()) / (t - t3.back()));
    else
      jerk3.push_back(0.0);
  }
  acc3.push_back(acc);
  t3.push_back(t);
}

void MainFrame::clearData1()
{
  pos1.clear();
  vel1.clear();
  acc1.clear();
  jerk1.clear();
  t1.clear();
}

void MainFrame::clearError1()
{
  pos_err1.clear();
  vel_err1.clear();
  acc_err1.clear();
  jerk_err1.clear();
  t_err1.clear();
}

void MainFrame::clearData2()
{
  pos2.clear();
  vel2.clear();
  acc2.clear();
  jerk2.clear();
  t2.clear();
}

void MainFrame::clearData3()
{
  pos3.clear();
  vel3.clear();
  acc3.clear();
  jerk3.clear();
  t3.clear();
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

void MainFrame::drawMultiGraphErrors(TMultiGraph *mg, TGraph *g, TLegend *leg, std::vector<double> &data1, std::vector<double> &data2, std::vector<double> &data3, EColor color, std::string label)
{
  std::vector<double> bar(data1.size(), 0.5);
  g = new TGraphErrors(static_cast<Int_t>(data1.size()), data1.data(), data2.data(), bar.data(), data3.data());
//  for (size_t i=0; i < data1.size(); i++)
//  {
//    Double_t nex = g->GetErrorX(static_cast<Int_t>(i));
//    Double_t ney = g->GetErrorY(static_cast<Int_t>(i)) /100.;
//    dynamic_cast<TGraphErrors*>(g)->SetPointError(static_cast<Int_t>(i), nex, ney);
//  }
  g->SetLineColor(color);
  g->SetLineWidth(1);
  mg->Add(g);
  auto maxit = std::minmax_element(data2.begin(), data2.end());
  auto absmax = {std::fabs(*maxit.first), std::fabs(*maxit.second)};
  std::string txt = label + " " + std::to_string(*std::max_element(absmax.begin(), absmax.end()));
  leg->AddEntry(g, txt.c_str());
}

void MainFrame::savePdfHisto(std::vector<double> &data1, const std::string save_file, const std::string title)
{
  TCanvas canvas("Canvas");

  TH1D *h1 = new TH1D("h1", title.c_str(), static_cast<Int_t>(data1.size()), 0.0, data1.back());
  for(size_t i = 0; i < data1.size(); i++)
      h1->Fill(data1.at(i));
  h1->SetTitle(title.c_str());
  h1->Draw();

  canvas.SetGrid();
  std::string fn = ros::package::getPath("rplot") + "/out/" + std::to_string(cnt++) + "_" + save_file;
  canvas.SaveAs(fn.c_str());
}

void MainFrame::savePdf(const bool plt_pos, const bool plt_vel, const bool plt_acc, const bool plt_jerk, const std::string save_file, const bool plt_data2, const bool plt_data3, const bool plt_error1, const std::string title1, const std::string title2, const std::string title3)
{
  if (t1.size() == 0 || cnt > 100)
  {
    return;
  }

  TCanvas canvas("Canvas", "Graph",200,10,700,500);
  canvas.SetFillColor(0);
  canvas.SetGrid();
  canvas.GetFrame()->SetFillColor(21);
  canvas.GetFrame()->SetBorderSize(12);
  if (plt_data2 && !plt_data3)
  {
    canvas.Divide(1,2);
    canvas.cd(1);
  }
  else if (plt_data3)
  {
    canvas.Divide(1,3);
    canvas.cd(1);
  }

  TMultiGraph mg1, mg2, mg3;
  TGraph *g11=nullptr, *g12=nullptr, *g13=nullptr, *g14=nullptr;
  TGraph *g21=nullptr, *g22=nullptr, *g23=nullptr, *g24=nullptr;
  TGraph *g31=nullptr, *g32=nullptr, *g33=nullptr, *g34=nullptr;
  TLegend leg1(.1,.7,.3,.9,"");
  leg1.SetFillColor(0);
  TLegend leg2 = leg1;
  TLegend leg3 = leg1;

  if (plt_pos && pos1.size())
  {
    if (!plt_error1)
      drawMultiGraph(&mg1, g11, &leg1, t1, pos1, kBlue, "Position");
    else
      drawMultiGraphErrors(&mg1, g11, &leg1, t1, pos1, pos_err1, kBlue, "Position");
  }
  if (plt_vel && vel1.size())
  {
    if (!plt_error1)
      drawMultiGraph(&mg1, g12, &leg1, t1, vel1, kGreen, "Velocity");
    else
      drawMultiGraphErrors(&mg1, g12, &leg1, t1, vel1, vel_err1, kGreen, "Velocity");
  }
  if (plt_acc && acc1.size())
  {
    if (!plt_error1)
      drawMultiGraph(&mg1, g13, &leg1, t1, acc1, kRed, "Acceleration");
    else
      drawMultiGraphErrors(&mg1, g13, &leg1, t1, acc1, acc_err1, kRed, "Acceleration");
  }
  if (plt_jerk && jerk1.size())
  {
    if (!plt_error1)
      drawMultiGraph(&mg1, g14, &leg1, t1, jerk1, kViolet, "Jerk");
    else
      drawMultiGraphErrors(&mg1, g14, &leg1, t1, jerk1, jerk_err1, kViolet, "Jerk");
  }
  mg1.SetTitle(title1.c_str());
  mg1.Draw("ALPZ");
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
    mg2.SetTitle(title2.c_str());
    mg2.Draw("ALP");
    leg2.Draw("Same");
  }

  if (plt_data3)
  {
    canvas.cd(3);
    if (plt_pos && pos3.size())
      drawMultiGraph(&mg3, g31, &leg3, t3, pos3, kBlue, "Position");
    if (plt_vel && vel3.size())
      drawMultiGraph(&mg3, g32, &leg3, t3, vel3, kGreen, "Velocity");
    if (plt_acc && acc3.size())
      drawMultiGraph(&mg3, g33, &leg3, t3, acc3, kRed, "Acceleration");
    if (plt_jerk && jerk3.size())
      drawMultiGraph(&mg3, g34, &leg3, t3, jerk3, kViolet, "Jerk");
    mg3.SetTitle(title3.c_str());
    mg3.Draw("ALP");
    leg3.Draw("Same");
  }

  std::string fn = ros::package::getPath("rplot") + "/out/" + std::to_string(cnt++) + "_" + save_file;
  canvas.Print(fn.c_str());

  clearData1();
  if (plt_data2)
    clearData2();
  if (plt_data3)
    clearData3();
  if (plt_error1)
    clearError1();
}
