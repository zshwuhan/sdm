/*******************************************************************************
 * Copyright (c) 2012, Dougal J. Sutherland (dsutherl@cs.cmu.edu).             *
 * All rights reserved.                                                        *
 *                                                                             *
 * Redistribution and use in source and binary forms, with or without          *
 * modification, are permitted provided that the following conditions are met: *
 *                                                                             *
 *     * Redistributions of source code must retain the above copyright        *
 *       notice, this list of conditions and the following disclaimer.         *
 *                                                                             *
 *     * Redistributions in binary form must reproduce the above copyright     *
 *       notice, this list of conditions and the following disclaimer in the   *
 *       documentation and/or other materials provided with the distribution.  *
 *                                                                             *
 *     * Neither the name of Carnegie Mellon University nor the                *
 *       names of the contributors may be used to endorse or promote products  *
 *       derived from this software without specific prior written permission. *
 *                                                                             *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" *
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   *
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE  *
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   *
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         *
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF        *
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    *
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN     *
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)     *
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE  *
 * POSSIBILITY OF SUCH DAMAGE.                                                 *
 ******************************************************************************/
#ifndef SDM_KERNEL_GAUSSIAN_HPP_
#define SDM_KERNEL_GAUSSIAN_HPP_
#include "sdm/basics.hpp"
#include "sdm/kernels/kernel.hpp"

#include <cmath>
#include <string>
#include <boost/format.hpp>

namespace sdm {

class GaussianKernel : public Kernel {
    typedef Kernel super;

protected:
    double sigma;

public:
    GaussianKernel(double sigma) : sigma(sigma) {}

    virtual std::string name() const;

    virtual double transformDivergence(double div) const;
    // TODO implement vectorized gaussian kernel

private:
    virtual GaussianKernel* do_clone() const;
};

////////////////////////////////////////////////////////////////////////////////

namespace detail {
    const double sigs[8] = { 1./16., 1./4., 1, 1<<2, 1<<4, 1<<6, 1<<8, 1<<10 };
}
const std::vector<double> default_sigmas(detail::sigs, detail::sigs + 8);


class GaussianKernelGroup : public KernelGroup {
protected:
    const std::vector<double> &sigmas;
    bool scale_sigma;

public:
    typedef GaussianKernel KernelType;

    GaussianKernelGroup(
            const std::vector<double> &sigmas = default_sigmas,
            bool scale_sigma = true)
        :
            sigmas(sigmas), scale_sigma(scale_sigma)
    {}

    const boost::ptr_vector<Kernel>* getTuningVector(
            const double* divs, size_t n) const;

private:
    virtual GaussianKernelGroup* do_clone() const;
};

} // end namespace
#endif
