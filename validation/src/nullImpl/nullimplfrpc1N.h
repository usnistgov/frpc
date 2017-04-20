/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties. Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility  whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#ifndef NULLIMPLFRPC1N_H_
#define NULLIMPLFRPC1N_H_

#include "frpc.h"

/*
 * Declare the implementation class of the FRPC IDENT (1:N) Interface
 */
namespace FRPC {
    class NullImplFRPC1N : public FRPC::IdentInterface {
public:

    NullImplFRPC1N();
    ~NullImplFRPC1N() override;

    ReturnStatus
    initializeEnrollmentSession(const std::string &configDir) override;

    ReturnStatus
    setGPU(uint8_t gpuNum) override;

    ReturnStatus
    createTemplate(
            const Image &face,
            TemplateRole role,
            std::vector<uint8_t> &templ,
            EyePair &eyeCoordinates) override;

    ReturnStatus
    finalizeEnrollment(
            const std::string &enrollmentDir,
            const std::string &edbName,
            const std::string &edbManifestName) override;

    ReturnStatus
    initializeProbeTemplateSession(
            const std::string &configDir,
            const std::string &enrollmentDir) override;

    ReturnStatus
    initializeIdentificationSession(
            const std::string &configDir,
            const std::string &enrollmentDir) override;

    ReturnStatus
    identifyTemplate(
            const std::vector<uint8_t> &idTemplate,
            const uint32_t candidateListLength,
            std::vector<Candidate> &candidateList,
            bool &decision) override;

    static std::shared_ptr<FRPC::IdentInterface>
    getImplementation();

private:
    std::string configDir;
    std::string enrollDir;
    std::vector<std::string> templateIds;
    uint8_t whichGPU;
    int counter;
    // Some other members
};
}

#endif /* NULLIMPLFRPC1N_H_ */
