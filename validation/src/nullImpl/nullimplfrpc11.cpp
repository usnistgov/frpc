/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties. Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility  whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#include <cstring>

#include "nullimplfrpc11.h"

using namespace std;
using namespace FRPC;

NullImplFRPC11::NullImplFRPC11() {}

NullImplFRPC11::~NullImplFRPC11() {}

ReturnStatus
NullImplFRPC11::initialize(const std::string &configDir)
{
    this->whichGPU = 0;
    this->counter = 0;
    return ReturnStatus(ReturnCode::Success);
}

ReturnStatus
NullImplFRPC11::setGPU(uint8_t gpuNum)
{
    this->whichGPU = gpuNum;
	return ReturnStatus(ReturnCode::Success);
}

ReturnStatus
NullImplFRPC11::createTemplate(
        const Image &face,
        TemplateRole role,
        std::vector<uint8_t> &templ,
        EyePair &eyeCoordinates)
{
    string blurb{"Somewhere out there, beneath the pale moon light\n"};

    templ.resize(blurb.size());
    memcpy(templ.data(), blurb.c_str(), blurb.size());
    eyeCoordinates = EyePair(true, true, 0, 0, 0, 0);

    return ReturnStatus(ReturnCode::Success);
}

ReturnStatus
NullImplFRPC11::matchTemplates(
        const std::vector<uint8_t> &verifTemplate,
        const std::vector<uint8_t> &enrollTemplate,
        double &similarity)
{
    similarity = 0.88;
    return ReturnStatus(ReturnCode::Success);
}

std::shared_ptr<VerifInterface>
VerifInterface::getImplementation()
{
    return std::make_shared<NullImplFRPC11>();
}





