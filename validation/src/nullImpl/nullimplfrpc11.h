/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties. Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility  whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#ifndef NULLIMPLFRPC11_H_
#define NULLIMPLFRPC11_H_

#include "frpc.h"

/*
 * Declare the implementation class of the FRPC VERIF (1:1) Interface
 */
namespace FRPC {
    class NullImplFRPC11 : public FRPC::VerifInterface {
public:

    NullImplFRPC11();
    ~NullImplFRPC11() override;

    ReturnStatus
    initialize(const std::string &configDir) override;

    ReturnStatus
    setGPU(uint8_t gpuNum) override;

    ReturnStatus
    createTemplate(
            const Image &face,
            TemplateRole role,
            std::vector<uint8_t> &templ,
            EyePair &eyeCoordinates) override;

    ReturnStatus
    matchTemplates(
            const std::vector<uint8_t> &verifTemplate,
            const std::vector<uint8_t> &enrollTemplate,
            double &similarity) override;

    static std::shared_ptr<FRPC::VerifInterface>
    getImplementation();

private:
    std::string configDir;
    uint8_t whichGPU;
    int counter;
    // Some other members
};
}

#endif /* NULLIMPLFRPC11_H_ */
