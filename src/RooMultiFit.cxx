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

std::unique_ptr<RooFitResult> performMultiFit(RooAbsPdf& pdf,
                                              RooRealVar& observable,
                                              const TH1& hist,
                                              const std::size_t nFits,
                                              const double smearingFraction) {
  RooDataHist dataHist("tempFitHist", "tempFitHist", observable, &hist);

  std::unique_ptr<RooArgSet> pParams = getParams(pdf, observable);

  std::unique_ptr<RooArgSet> pBestParams =
      std::unique_ptr<RooArgSet>(pParams->snapshot());
  std::unique_ptr<RooFitResult> pBestResult;
  double bestNll = std::numeric_limits<double>::infinity();

  for (std::size_t i = 0; i < nFits; i++) {
    pParams->assign(*pBestParams);
    smearParams(*pParams, smearingFraction);

    auto pResult = std::unique_ptr<RooFitResult>(
        pdf.fitTo(dataHist, RooFit::Save(), RooFit::PrintLevel(-1)));
    const double nll = pResult->minNll();

    if (nll < bestNll) {
      pBestParams.reset(pParams->snapshot());
      pBestResult = std::move(pResult);
      bestNll = nll;
    }
  }

  pParams->assign(*pBestParams);

  return pBestResult;
}

}  // namespace RooMultiFit
}  // namespace RooFitUtils
