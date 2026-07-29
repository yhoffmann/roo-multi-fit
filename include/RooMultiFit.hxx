#ifndef MULTIFIT_ROOMULTIFIT_CXX_
#define MULTIFIT_ROOMULTIFIT_CXX_

#include <RooAbsPdf.h>
#include <RooArgSet.h>
#include <RooFitResult.h>
#include <RooRealVar.h>
#include <TH1.h>
#include <memory>

namespace RooFitUtils {
namespace RooMultiFit {

std::unique_ptr<RooArgSet> getParams(RooAbsPdf& pdf, RooRealVar& observable);

void smearParam(RooRealVar& pParam, double smearingFraction);

void smearParams(RooArgSet& params, const double smearingFraction = 0.15);

std::unique_ptr<RooFitResult> performMultiFit(
    RooAbsPdf& pdf, RooRealVar& observable, const TH1& hist,
    const std::size_t nFits, const double smearingFraction = 0.15);

}  // namespace RooMultiFit
}  // namespace RooFitUtils

#endif  // MULTIFIT_ROOMULTIFIT_CXX_
