/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties. Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility  whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>

#include "nullimplfrpc1N.h"

using namespace std;
using namespace FRPC;

NullImplFRPC1N::NullImplFRPC1N() {}

NullImplFRPC1N::~NullImplFRPC1N() {}

ReturnStatus
NullImplFRPC1N::initializeEnrollmentSession(const string &configDir)
{
    this->whichGPU = 0;
    this->counter = 0;
    this->configDir = configDir;
    return ReturnStatus(ReturnCode::Success);
}

ReturnStatus
NullImplFRPC1N::setGPU(uint8_t gpuNum)
{
    this->whichGPU = gpuNum;
	return ReturnStatus(ReturnCode::Success);
}

ReturnStatus
NullImplFRPC1N::createTemplate(
        const Image &face,
        TemplateRole role,
        vector<uint8_t> &templ,
        EyePair &eyeCoordinates)
{
    string blurb{"Somewhere out there, beneath the pale moon light\n"};

    templ.resize(blurb.size());
    memcpy(templ.data(), blurb.c_str(), blurb.size());
    eyeCoordinates = EyePair(true, true, 0, 0, 0, 0);

    return ReturnStatus(ReturnCode::Success);
}

ReturnStatus
NullImplFRPC1N::finalizeEnrollment(
        const string &enrollmentDir,
        const string &edbName,
        const string &edbManifestName)
{
    ifstream edbsrc(edbName, ios::binary);
    ofstream edbdest(enrollmentDir+"/mei.edb", ios::binary);
    ifstream manifestsrc(edbManifestName, ios::binary);
    ofstream manifestdest(enrollmentDir+"/mei.manifest", ios::binary);

    edbdest << edbsrc.rdbuf();
    manifestdest << manifestsrc.rdbuf();

    return ReturnCode::Success;
}

ReturnStatus
NullImplFRPC1N::initializeProbeTemplateSession(
        const string &configDir,
        const string &enrollmentDir)
{
    this->configDir = configDir;
    this->enrollDir = enrollmentDir;

    /* Load stuff from enrollment database into memory, read in
     * configuration files, etc
     */
    return ReturnCode::Success;
}

ReturnStatus
NullImplFRPC1N::initializeIdentificationSession(
        const string &configDir,
        const string &enrollmentDir)
{
    this->configDir = configDir;
    this->enrollDir = enrollmentDir;

    /* Load stuff from enrollment database into memory, read in
     * configuration files, etc
     */
    /* Read input file */
    auto manifest = enrollmentDir + "/mei.manifest";
    ifstream inputStream(manifest);
    if (!inputStream.is_open()) {
        cerr << "Failed to open stream for " << manifest << "." << endl;
        return ReturnCode::EnrollDirError;
    }

    string id, size, offset;
    while (inputStream >> id >> size >> offset) {
        templateIds.push_back(id);
    }

    return ReturnCode::Success;
}

ReturnStatus
NullImplFRPC1N::identifyTemplate(
        const vector<uint8_t> &idTemplate,
        const uint32_t candidateListLength,
        vector<Candidate> &candidateList,
        bool &decision)
{
    for (unsigned int i=0; i<candidateListLength; i++) {
        candidateList.push_back(Candidate(true, templateIds[i%(templateIds.size()-1)], candidateListLength-i));
    }
    decision = true;
    return ReturnCode::Success;
}

shared_ptr<IdentInterface>
IdentInterface::getImplementation()
{
    return make_shared<NullImplFRPC1N>();
}





