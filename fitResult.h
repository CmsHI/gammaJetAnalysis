#ifndef fitResult_h
#define fitResult_h

#include <TH1.h>
#include <TNtuple.h>
#include <TCut.h>
#include <TF1.h>

class fitResult {
public:
  Double_t nSig;
  Double_t nSigErr;
  Double_t purity;
  Double_t chisq;
  Double_t sigMeanShift;

  TH1F *sigPdf;
  TH1F *bckPdf;
  TH1D *data;

  static fitResult doFit(TH1D* hSig=0, TH1D* hBkg=0, TH1D* hData1=0,
			 Float_t varLow=0.001, Float_t varHigh=0.028,
			 Double_t purityBinVal=0.00999);

  static fitResult getPurity(TTree *dataTree, TTree *mcTree,
			     TCut dataCandidateCut, TCut sidebandCut,
			     TCut mcSignalCut, Double_t signalShift=0.0,
			     Double_t backgroundShift=0.0, Double_t purityBinVal=0.00999);
private:
  void mcStyle(TH1* h);
  void sbStyle(TH1* h);

};

class fitHistFunction
{
public:
  fitHistFunction(TH1D *h, TH1D *h2);
  ~fitHistFunction(){
    delete histBck;
    delete histSig;
  };

  //      Int_t GetBinNumber(Double_t c);
  Double_t evaluate(Double_t *x, Double_t *par);
  TH1D *histSig;
  TH1D *histBck;
  Double_t lowEdge;
  Double_t highEdge;
  Double_t nbin;
};

fitHistFunction::fitHistFunction(TH1D *h,TH1D *h2)
{
  histSig=(TH1D*)h->Clone();
  histBck=(TH1D*)h2->Clone();

  nbin=h->GetNbinsX();
  lowEdge=h->GetBinLowEdge(1);
  highEdge=h->GetBinLowEdge(nbin+1);

  histSig->SetName("hSig");
  histSig->Scale(1./histSig->Integral(1,histSig->GetNbinsX()+1));
  histBck->SetName("hBck");
  histBck->Scale(1./histBck->Integral(1,histBck->GetNbinsX()+1));
}

Double_t fitHistFunction::evaluate(Double_t *x, Double_t *par) {

  Double_t xx = x[0];
  Int_t binNum=histSig->FindBin(xx);  //

  return par[0]*(histSig->GetBinContent(binNum)*par[1]+histBck->GetBinContent(binNum)*(1-par[1]));
}


fitResult fitResult::doFit(TH1D* hSig, TH1D* hBkg, TH1D* hData1,
			   Float_t varLow, Float_t varHigh,
			   Double_t purityBinVal)
{
  TH1D* hDatatmp = (TH1D*)hData1->Clone(Form("%s_datatmp",hData1->GetName()));
  Int_t nBins = hDatatmp->GetNbinsX();
  fitHistFunction *myFits = new fitHistFunction(hSig,hBkg);
  TF1 *f = new TF1("f",myFits,&fitHistFunction::evaluate,varLow,varHigh,2);
  f->SetParameters( hDatatmp->Integral(1,nBins+1), 0.3);
  f->SetParLimits(1,0,1);
  hDatatmp->Fit("f","WL M 0 Q","",varLow,varHigh);
  hDatatmp->Fit("f","WL M 0 Q","",varLow,varHigh);

  fitResult res;
  res.nSig =0;
  Double_t nev = f->GetParameter(0);
  Double_t ratio = f->GetParameter(1);
  Double_t ratioErr = f->GetParError(1);
  res.nSig    = nev * ratio;
  res.nSigErr = nev * ratioErr;
  res.chisq = (Double_t)f->GetChisquare()/ f->GetNDF();

  TH1F *hSigPdf = (TH1F*)hSig->Clone(Form("%s_tmp",hSig->GetName()));
  hSigPdf->Scale(res.nSig/hSigPdf->Integral(1,nBins+1));

  TH1F *hBckPdf = (TH1F*)hBkg->Clone(Form("%s_tmp",hBkg->GetName()));
  hBckPdf->Scale((nev-res.nSig)/hBckPdf->Integral(1,nBins+1));

  Double_t ss1 = hSigPdf->Integral(1, hSigPdf->FindBin(purityBinVal),"width");
  Double_t bb1 = hBckPdf->Integral(1, hBckPdf->FindBin(purityBinVal),"width");
  res.purity = ss1/(ss1+bb1);

  res.sigPdf = (TH1F*)hSigPdf->Clone(Form("%s_sig",hSig->GetName()));
  res.bckPdf = (TH1F*)hBckPdf->Clone(Form("%s_bck",hBkg->GetName()));
  res.data = (TH1D*)hData1->Clone(Form("%s_cand",hData1->GetName()));

  return res;
}

void fitResult::mcStyle(TH1* h) {
  h->SetLineColor(kPink);
  h->SetFillColor(kOrange+7);
  h->SetFillStyle(3004);
}

void fitResult::sbStyle(TH1* h) {
  h->SetLineColor(kGreen+4);
  h->SetFillColor(kGreen+1);
  h->SetFillStyle(3001);
}


//const Int_t nSIGMABINS = 100; // number of bins in sigmaIetaIeta dist
const Int_t nSIGMABINS = 25;
const Double_t maxSIGMA = 0.025; // x-axis max of sigmaIetaIeta dist

fitResult fitResult::getPurity(TTree *dataTree, TTree *mcTree,
			       TCut dataCandidateCut, TCut sidebandCut,
			       TCut mcSignalCut, Double_t signalShift,
			       Double_t backgroundShift, Double_t purityBinVal)
{
  TH1D* hCand = new TH1D("cand","",nSIGMABINS,0,maxSIGMA);
  TH1D* hBkg = (TH1D*)hCand->Clone("bkg");
  TH1D* hSig = (TH1D*)hCand->Clone("sig");

  TString sigshift = "+";
  sigshift += signalShift;
  TString bkgshift = "+";
  bkgshift += backgroundShift;

  //Int_t dEntries = dataTree->Project(hCand->GetName(), "sigmaIetaIeta", dataCandidateCut, "");
  //Int_t sbEntries = dataTree->Project(hBkg->GetName(), "sigmaIetaIeta"+bkgshift, sidebandCut, "");
  //Int_t mcEntries = mcTree->Project(hSig->GetName(), "sigmaIetaIeta"+sigshift, "mcweight"*mcSignalCut, "");

  //cout << "# Candidates: " << dEntries << endl;
  //cout << "# Sideband: " << sbEntries << endl;
  //cout << "# MC Signal: " << mcEntries << endl;

  dataTree->Project(hCand->GetName(), "sigmaIetaIeta", dataCandidateCut, "");

  //dataTree->Project(hBkg->GetName(), "sigmaIetaIeta"+bkgshift, "(1-40*(sigmaIetaIeta-0.01))"*sidebandCut, "");
  dataTree->Project(hBkg->GetName(), "sigmaIetaIeta"+bkgshift, sidebandCut, "");
  //TFile *bkgFile = TFile::Open("gammaJets_pp_MC_EmEnrichedDijet.root");
  //TTree *bkgTree = (TTree*)bkgFile->Get("photonTree");
  //bkgTree->Project(hBkg->GetName(), "sigmaIetaIeta"+bkgshift, "mcweight"*sidebandCut, "");

  mcTree->Project(hSig->GetName(), "sigmaIetaIeta"+sigshift, "photonTree.mcweight"*mcSignalCut, "");

  //TCanvas *fakeCanvas = new TCanvas("fake","fake");
  fitResult fitr = fitResult::doFit(hSig, hBkg, hCand, 0.005, 0.035, purityBinVal);
  //delete fakeCanvas;

  delete hSig;
  delete hBkg;
  delete hCand;

  fitr.sigMeanShift = signalShift;

  return fitr;
}

#endif
