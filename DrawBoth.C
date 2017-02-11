#include <fstream>
#include <vector>

DrawBoth(double which) {
  ifstream infileN(Form("Output-neutron-%.6fMeV.txt", which));
  ifstream infileP(Form("Output[photon]-neutron-%.6fMeV.txt", which));

  double a, b, c;
  vector<double> aa, bb, cc;
  unsigned int N = 0;
  while (infileN >> a >> b >> c) {
    aa.push_back(a);
    bb.push_back(b);
    cc.push_back(c);
    N++;
  }
  const unsigned int sizeN = N - 1;

  vector<double> ccP;
  while(infileP >> a >> b >> c) {
    ccP.push_back(c);
  }

  double* light   = new double[sizeN];
  double* neutron = new double[sizeN];
  double* photon  = new double[sizeN];
  for (unsigned int i = 1; i < sizeN + 1; i++) {
    *(light + i - 1) = (aa.at(i) + bb.at(i)) / 2;
    *(neutron + i - 1) = cc.at(i);
    *(photon + i - 1) = ccP.at(i);
  }

  TGraph* grN = new TGraph(sizeN, light, neutron);
  grN->SetTitle("Light Response from Neutron Interactions");
  grN->GetXaxis()->SetTitle("Light Output");
  grN->GetYaxis()->SetTitle("Entries");

  TGraph* grP = new TGraph(sizeN, light, photon);
  grP->SetTitle("Light Response due to optical photons");
  grP->GetXaxis()->SetTitle("Light Output");
  grP->GetYaxis()->SetTitle("Entries");

  TCanvas *canvas = new TCanvas("canvas", "canvas", 900, 900);
  canvas->Divide(1, 2);
  canvas->cd(1);
  gPad->SetLogx();
  gPad->SetLogy();
  grN->Draw("APL");
  canvas->cd(2);
  gPad->SetLogx();
  gPad->SetLogy();
  grP->Draw("APL");
  canvas->SaveAs(Form("InNeutron_%.6fMeV.eps", which));

  return;
}
