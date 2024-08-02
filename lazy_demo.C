#include "TLazyPrimitive.h"

{
   TH1I *h1 = new TH1I("h1", "h1", 200, -5, 5);
   h1->FillRandom("gaus", 100000);

   TCanvas *can = new TCanvas("can", "can", 800, 800);
   h1->Draw();

   TLazyPrimitive *lazy = new TLazyPrimitive("fitting", "Lazy object example");
   lazy->SetFcn([] {
      auto h = dynamic_cast<TH1 *>(gROOT->Get("h1"));
      h->Fit("gaus", "", "", -5, 5);
   });
   lazy->Draw();

   can->BuildLegend();

   can->GetListOfPrimitives()->Print();

   can->SaveAs("can.root");
}
