#ifndef ROOT_TLazyPrimitive
#define ROOT_TLazyPrimitive

#include "TNamed.h"

#include <functional>

using fcn_t = void(void);
class TLazyPrimitive : public TNamed {

private:
   std::function<fcn_t> lazy_fnc;

public:
   TLazyPrimitive() : TNamed() {}
   TLazyPrimitive(const char *name, const char *title) : TNamed(name, title) {}
   TLazyPrimitive(const char *name, const char *title, fcn_t fcn) : TNamed(name, title), lazy_fnc(std::move(fcn)) {}

   virtual ~TLazyPrimitive() {}

   void SetFcn(fcn_t fcn) { lazy_fnc = std::move(fcn); }

   void Draw(Option_t *option = "") override
   {
      TString opt1 = option;
      opt1.ToLower();
      TString opt2 = option;
      Int_t index = opt1.Index("same");

      // Check if the string "same" is part of a TCutg name.
      if (index >= 0) {
         Int_t indb = opt1.Index("[");
         if (indb >= 0) {
            Int_t indk = opt1.Index("]");
            if (index > indb && index < indk)
               index = -1;
         }
      }

      // If there is no pad or an empty pad the "same" option is ignored.
      if (gPad) {
         if (!gPad->IsEditable())
            gROOT->MakeDefCanvas();
         if (index >= 0) {
            if (gPad->GetX1() == 0 && gPad->GetX2() == 1 && gPad->GetY1() == 0 && gPad->GetY2() == 1 &&
                gPad->GetListOfPrimitives()->GetSize() == 0)
               opt2.Remove(index, 4);
         } else {
            // the following statement is necessary in case one attempts to draw
            // a temporary histogram already in the current pad
            if (TestBit(kCanDelete))
               gPad->GetListOfPrimitives()->Remove(this);
            gPad->Clear();
         }
         gPad->IncrementPaletteColor(1, opt1);
      } else {
         if (index >= 0)
            opt2.Remove(index, 4);
      }

      AppendPad(opt2.Data());

      lazy_fnc();

      gPad->GetListOfPrimitives()->Add(this);
   }
};

#endif /* ROOT_TLazyPrimitive */
