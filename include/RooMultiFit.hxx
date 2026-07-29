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

/**
 * @brief Function that performs a fit multiple times, returing the best result
 *
 * @param [in] pdf The model to fit to the histogram
 * @param [in] observable The parameter to fit in
 * @param hist The histogram to fit the model to
 * @param nFits The number of separate fits to perform
 * @param smearingFraction (optional) Value governing the range of smearing
 *
 * @returns The best fit result after the `nFits` fits
 *
 * @details Perform a fit of `pdf` to `hist` `nFits` times, each time smearing
 * the effective parameters within their error (or `smearingFraction` of the
 * total possible range). The best fit will be kept track of by comparing the
 * Nll of different results. Will return the best result and, of course, also
 * change the parameters to their best values before returning.
 */
std::unique_ptr<RooFitResult> performMultiFit(
    RooAbsPdf& pdf, RooRealVar& observable, const TH1& hist,
    const std::size_t nFits, const double smearingFraction = 0.15);

}  // namespace RooMultiFit
}  // namespace RooFitUtils

#endif  // MULTIFIT_ROOMULTIFIT_CXX_
