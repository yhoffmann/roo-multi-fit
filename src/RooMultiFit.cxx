#include "../include/RooMultiFit.hxx"

#include <RooDataHist.h>
#include <RooRandom.h>
#include <limits>

namespace RooFitUtils {
namespace RooMultiFit {

std::unique_ptr<RooArgSet> getParams(RooAbsPdf& pdf, RooRealVar& observable) {
  return std::unique_ptr<RooArgSet>(pdf.getParameters(observable));
}

void smearParam(RooRealVar& param, double smearingFraction) {
  const double error = param.getError();
  const double totalRange =
      smearingFraction * (param.getMax() - param.getMin());

  const double smearingSigma =
      (error > 0.0 && error > totalRange) ? error : totalRange;

  const double smeared =
      RooRandom::randomGenerator()->Gaus(param.getVal(), smearingSigma);

  param.setVal(std::clamp(smeared, param.getMin(), param.getMax()));
}

void smearParams(RooArgSet& params, const double smearingFraction) {
  for (auto pObj : params) {
    auto* pParam = dynamic_cast<RooRealVar*>(pObj);

    if (nullptr == pParam || pParam->isConstant()) continue;

    smearParam(*pParam, smearingFraction);
  }
}
}  // namespace RooMultiFit
}  // namespace RooFitUtils
