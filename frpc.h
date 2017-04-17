/*
 * This software was developed at the National Institute of Standards and
 * Technology (NIST) by employees of the Federal Government in the course
 * of their official duties. Pursuant to title 17 Section 105 of the
 * United States Code, this software is not subject to copyright protection
 * and is in the public domain. NIST assumes no responsibility whatsoever for
 * its use by other parties, and makes no guarantees, expressed or implied,
 * about its quality, reliability, or any other characteristic.
 */

#ifndef FRPC_H_
#define FRPC_H_

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace FRPC {
/**
 * @brief
 * Struct representing a single image
 */
typedef struct Image {
    /** Number of pixels horizontally */
    uint16_t width;
    /** Number of pixels vertically */
    uint16_t height;
    /** Number of bits per pixel. Legal values are 8 and 24. */
    uint8_t depth;
    /** Managed pointer to raster scanned data.
     * Either RGB color or intensity.
     * If image_depth == 24 this points to  3WH bytes  RGBRGBRGB...
     * If image_depth ==  8 this points to  WH bytes  IIIIIII */
    std::shared_ptr<uint8_t> data;

    Image() :
        width{0},
        height{0},
        depth{24}
        {}

    Image(
        uint16_t width,
        uint16_t height,
        uint8_t depth,
        const std::shared_ptr<uint8_t> &data
        ) :
        width{width},
        height{height},
        depth{depth},
        data{data}
        {}

    /** @brief This function returns the size of the image data. */
    size_t
    size() const { return (width * height * (depth / 8)); }
} Image;


/** Labels describing the type/role of the template
 * to be generated (provided as input to template generation)
 */
enum class TemplateRole {
    /** Enrollment template for 1:N */
    Enrollment_1N,
    /** Search template for 1:N */
    Search_1N,
    /** Enrollment template for 1:1 */
    Enrollment_11,
    /** Verification template for 1:1 */
    Verification_11
};

/**
 * @brief
 * Return codes for functions specified in this API
 */
enum class ReturnCode {
    /** Success */
    Success = 0,
    /** Error reading configuration files */
    ConfigError,
    /** Elective refusal to process the input */
    RefuseInput,
    /** Involuntary failure to process the image */
    ExtractError,
    /** Cannot parse the input data */
    ParseError,
    /** Elective refusal to produce a template */
    TemplateCreationError,
    /** Either or both of the input templates were result of failed
     * feature extraction */
    VerifTemplateError,
    /** The implementation cannot support the number of input images */
    NumDataError,
    /** Template file is an incorrect format or defective */
    TemplateFormatError,
    /**
     * An operation on the enrollment directory
     * failed (e.g. permission, space)
     */
    EnrollDirError,
    /** Cannot locate the input data - the input
    * files or names seem incorrect
    */
    InputLocationError,
	/** There was a problem setting or accessing the GPU */
	GPUError,
    /** Vendor-defined failure */
    VendorError
};

/** Output stream operator for a ReturnCode object. */
inline std::ostream&
operator<<(
    std::ostream &s,
    const ReturnCode &rc)
{
    switch (rc) {
    case ReturnCode::Success:
        return (s << "Success");
    case ReturnCode::ConfigError:
        return (s << "Error reading configuration files");
    case ReturnCode::RefuseInput:
        return (s << "Elective refusal to process the input");
    case ReturnCode::ExtractError:
        return (s << "Involuntary failure to process the image");
    case ReturnCode::ParseError:
        return (s << "Cannot parse the input data");
    case ReturnCode::TemplateCreationError:
        return (s << "Elective refusal to produce a template");
    case ReturnCode::VerifTemplateError:
        return (s << "Either/both input templates were result of failed feature extraction");
    case ReturnCode::NumDataError:
        return (s << "Number of input images not supported");
    case ReturnCode::TemplateFormatError:
        return (s << "Template file is an incorrect format or defective");
    case ReturnCode::EnrollDirError:
            return (s << "An operation on the enrollment directory failed");
    case ReturnCode::InputLocationError:
            return (s << "Cannot locate the input data - the input file or names seem incorrect");
	case ReturnCode::GPUError:
		return (s << "Problem setting or accessing the GPU");
    case ReturnCode::VendorError:
        return (s << "Vendor-defined error");
    default:
        return (s << "Undefined error");
    }
}

/**
 * @brief
 * A structure to contain information about a failure by the software
 * under test.
 *
 * @details
 * An object of this class allows the software to return some information
 * from a function call. The string within this object can be optionally
 * set to provide more information for debugging etc. The status code
 * will be set by the function to Success on success, or one of the
 * other codes on failure.
 */
typedef struct ReturnStatus {
    /** @brief Return status code */
    ReturnCode code;
    /** @brief Optional information string */
    std::string info;

    ReturnStatus() {}
    /**
     * @brief
     * Create a ReturnStatus object.
     *
     * @param[in] code
     * The return status code; required.
     * @param[in] info
     * The optional information string.
     */
    ReturnStatus(
        const ReturnCode code,
        const std::string &info = ""
        ) :
        code{code},
        info{info}
        {}
} ReturnStatus;

typedef struct EyePair
{
    /** If the left eye coordinates have been computed and
     * assigned successfully, this value should be set to true,
     * otherwise false. */
    bool isLeftAssigned;
    /** If the right eye coordinates have been computed and
     * assigned successfully, this value should be set to true,
     * otherwise false. */
    bool isRightAssigned;
    /** X and Y coordinate of the center of the subject's left eye.  If the
     * eye coordinate is out of range (e.g. x < 0 or x >= width), isLeftAssigned
     * should be set to false. */
    uint16_t xleft;
    uint16_t yleft;
    /** X and Y coordinate of the center of the subject's right eye.  If the
     * eye coordinate is out of range (e.g. x < 0 or x >= width), isRightAssigned
     * should be set to false. */
    uint16_t xright;
    uint16_t yright;

    EyePair() :
        isLeftAssigned{false},
        isRightAssigned{false},
        xleft{0},
        yleft{0},
        xright{0},
        yright{0}
        {}

    EyePair(
        bool isLeftAssigned,
        bool isRightAssigned,
        uint16_t xleft,
        uint16_t yleft,
        uint16_t xright,
        uint16_t yright
        ) :
        isLeftAssigned{isLeftAssigned},
        isRightAssigned{isRightAssigned},
        xleft{xleft},
        yleft{yleft},
        xright{xright},
        yright{yright}
        {}
} EyePair;

/**
 * @brief
 * Data structure for result of an identification search
 */
typedef struct Candidate {
    /** @brief If the candidate is valid, this should be set to true. If
     * the candidate computation failed, this should be set to false. */
    bool isAssigned;

    /** @brief The template ID from the enrollment database manifest */
    std::string templateId;

    /** @brief Measure of similarity between the identification template
     * and the enrolled candidate.  Higher scores mean more likelihood that
     * the samples are of the same person.  An algorithm is free to assign
     * any value to a candidate.
     * The distribution of values will have an impact on the appearance of a
     * plot of false-negative and false-positive identification rates. */
    double similarityScore;

    Candidate() :
        isAssigned{false},
        templateId{""},
        similarityScore{0.0}
        {}

    Candidate(
        bool isAssigned,
        std::string templateId,
        double similarityScore) :
        isAssigned{isAssigned},
        templateId{templateId},
        similarityScore{similarityScore}
        {}
} Candidate;

/* API functions to be implemented */

/**
 * @brief
 * The interface to FRPC Challenge IDENT 1:N implementation
 *
 * @details
 * The submission software under test will implement this interface by
 * sub-classing this class and implementing each method therein.
 */
class IdentInterface {
public:
    virtual ~IdentInterface() {}

    /** @brief This function initializes the implementation under test and sets
     * all needed parameters.
     *
     * @details This function will be called N=1 times by the NIST application,
     * prior to parallelizing M >= 1 calls to createTemplate() via fork().
     *
     * @param[in] configDir
     * A read-only directory containing any developer-supplied configuration
     * parameters or run-time data files.
     */
    virtual ReturnStatus
    initializeEnrollmentSession(
        const std::string &configDir) = 0;

    /**
     * @brief This function takes an Image and outputs a template
     *
     * @details For enrollment templates: If the function
     * executes correctly (i.e. returns a successful exit status), the NIST
     * calling application will store the template.  The NIST application will
     * concatenate the templates and pass the result to the enrollment
     * finalization function.  When the implementation fails to produce a
     * template, it shall still return a blank template (which can be zero
     * bytes in length). The template will be included in the
     * enrollment database/manifest like all other enrollment templates, but
     * is not expected to contain any feature information.
     * <br>For identification templates: If the function returns a
     * non-successful return status, the output template will be not be used
     * in subsequent search operations.
     *
     * @param[in] face
     * The input face image.
     * @param[in] templateType
     * A value from the TemplateRole enumeration that indicates the intended
     * usage of the template to be generated.  In this case, either an
     * enrollment template used for gallery enrollment or an identification
     * template used for search.
     * @param[out] templ
     * The output template.  The format is entirely unregulated.  This will be
     * an empty vector when passed into the function, and the implementation can
     * resize and populate it with the appropriate data.
     * @param[out] eyeCoordinates
     * (Optional) The function may choose to return the estimated eye centers
     * for the input face image.
     */
    virtual ReturnStatus
    createTemplate(
        const Image &face,
        TemplateRole role,
        std::vector<uint8_t> &templ,
        EyePair &eyeCoordinates) = 0;

    /**
     * @brief This function will be called after all enrollment templates have
     * been created and freezes the enrollment data.
     * After this call, the enrollment dataset will be forever read-only.
     *
     * @details This function allows the implementation to conduct,
     * for example, statistical processing of the feature data, indexing and
     * data re-organization.  The function may create its own data structure.
     * It may increase or decrease the size of the stored data.  No output is
     * expected from this function, except a return code.  The function will
     * generally be called in a separate process after all the enrollment processes
     * are complete.
     * NOTE: Implementations shall not move the input data.  Implementations
     * shall not point to the input data.
     * Implementations should not assume the input data would be readable
     * after the call.  Implementations must,
     * <b>at a minimum, copy the input data</b> or otherwise extract what is
     * needed for search.
     *
     * @param[in] enrollmentDir
     * The top-level directory in which enrollment data was placed. This
     * variable allows an implementation
     * to locate any private initialization data it elected to place in the
     * directory.
     * @param[in] edbName
     * The name of a single file containing concatenated templates, i.e. the
     * EDB described in <em>File structure for enrolled template collection</em>.
     * While the file will have read-write-delete permission, the implementation
     * should only alter the file if it preserves the necessary content, in
     * other files for example.
     * The file may be opened directly.  It is not necessary to prepend a
     * directory name.  This is a NIST-provided
     * input - implementers shall not internally hard-code or assume any values.
     * @param[in] edbManifestName
     * The name of a single file containing the EDB manifest described in
     * <em>File structure for enrolled template collection</em>.
     * The file may be opened directly.  It is not necessary to prepend a
     * directory name.  This is a NIST-provided
     * input - implementers shall not internally hard-code or assume any values.
     */
    virtual ReturnStatus
    finalizeEnrollment(
        const std::string &enrollmentDir,
        const std::string &edbName,
        const std::string &edbManifestName) = 0;

    /**
     * @brief Before images are sent to the probe template
     * creation function, the test harness will call this initialization
     * function.
     *
     * @details This function initializes the implementation
     * under test and sets all needed parameters.  This function will be
     * called N=1 times by the NIST application,
     * prior to parallelizing M >= 1 calls to createTemplate() via fork().
     * The implementation has read-only access to its prior enrollment data.
     *
     * @param[in] configDir
     * A read-only directory containing any developer-supplied configuration
     * parameters or run-time data files.
     * @param[in] enrollmentDir
     * The read-only top-level directory in which enrollment data was placed
     * and then finalized by the implementation.
     * The implementation can parameterize subsequent template production on
     * the basis of the enrolled dataset.
     */
    virtual ReturnStatus
    initializeProbeTemplateSession(
        const std::string &configDir,
        const std::string &enrollmentDir) = 0;

    /** @brief This function will be called once prior to one or more calls to
     * identifyTemplate().  The function might set static internal variables
     * so that the enrollment database is available to the subsequent
     * identification searches.
     *
     * @param[in] configDir
     * A read-only directory containing any developer-supplied configuration
     * parameters or run-time data files.
     * @param[in] enrollmentDir
     * The read-only top-level directory in which enrollment data was placed.
     */
    virtual ReturnStatus
    initializeIdentificationSession(
        const std::string &configDir,
        const std::string &enrollmentDir) = 0;

    /** @brief This function searches an identification template against the
     * enrollment set, and outputs a
     * vector containing candidateListLength Candidates.
     *
     * @details Each candidate shall be populated by the implementation
     * and added to candidateList.  Note that candidateList will be an empty
     * vector when passed into this function.  The candidates shall appear in
     * descending order of similarity score - i.e. most similar entries appear
     * first.
     *
     * @param[in] idTemplate
     * A template from createTemplate().  If the value returned by that
     * function was non-successful, the contents of idTemplate will not be
     * used, and this function will not be called.
     * @param[in] candidateListLength
     * The number of candidates the search should return.
     * @param[out] candidateList
     * Each candidate shall be populated by the implementation.  The candidates
     * shall appear in descending order of similarity score - i.e. most similar
     * entries appear first.
     * @param[out] decision
     * A best guess at whether there is a mate within the enrollment database.
     * If there was a mate found, this value should be set to true, Otherwise, false.
     * Many such decisions allow a single point to be plotted alongside a DET curve.
     */
    virtual ReturnStatus
    identifyTemplate(
        const std::vector<uint8_t> &idTemplate,
        const uint32_t candidateListLength,
        std::vector<Candidate> &candidateList,
        bool &decision) = 0;

    /**
     * @brief This function sets the GPU device number to be used by all
     * subsequent implementation function calls.  gpuNum is a zero-based
     * sequence value of which GPU device to use.  0 would mean the first
     * detected GPU, 1 would be the second GPU, etc.  If the implementation
     * does not use GPUs, then this function call should simply do nothing.
     *
     * @param[in] gpuNum
     * Index number representing which GPU to use
     */
    virtual ReturnStatus
    setGPU(uint8_t gpuNum) = 0;

    /**
     * @brief
     * Factory method to return a managed pointer to the IdentInterface
     * object.
     *
     * @details
     * This function is implemented by the submitted library and must return
     * a managed pointer to the IdentInterface object.
     *
     * @note
     * A possible implementation might be:
     * return (std::make_shared<Implementation>());
     */
    static std::shared_ptr<IdentInterface>
    getImplementation();
};
/* End of IdentInterface */

/**
 * @brief
 * The interface to FRPC Challenge VERIF 1:1 implementation
 *
 * @details
 * The submission software under test will implement this interface by
 * sub-classing this class and implementing each method therein.
 */
class VerifInterface {
public:
    virtual ~VerifInterface() {}

    /**
     * @brief This function initializes the implementation under test.  It will
     * be called by the NIST application before any call to createTemplate() or
     * matchTemplates().  The implementation under test should set all parameters.
     * This function will be called N=1 times by the NIST application, prior to
     * parallelizing M >= 1 calls to createTemplate() via fork().
     *
     * @param[in] configDir
     * A read-only directory containing any developer-supplied configuration
     * parameters or run-time data files.  The name of this directory is
     * assigned by NIST, not hardwired by the provider.  The names of the
     * files in this directory are hardwired in the implementation and are
     * unrestricted.
     */
    virtual ReturnStatus
    initialize(const std::string &configDir) = 0;

    /**
     * @brief This function takes an Image and outputs a proprietary template
     * and associated eye coordinates.  The vector to store the template will be
     * initially empty, and it is up to the implementation
     * to populate them with the appropriate data.  In all cases, even when unable
     * to extract features, the output shall be a template that may be passed to
     * the match_templates function without error.  That is, this routine must
     * internally encode "template creation failed" and the matcher must
     * transparently handle this.
     *
     * param[in] face
     * Input face image
     * param[in] role
     * Label describing the type/role of the template to be generated
     * param[out] templ
     * The output template.  The format is entirely unregulated.  This will be
     * an empty vector when passed into the function, and the implementation
     * can resize and populate it with the appropriate data.
     * param[out] eyeCoordinates
     * The function shall return the estimated eye centers for the input image.
     */
    virtual ReturnStatus
    createTemplate(
        const Image &face,
        TemplateRole role,
        std::vector<uint8_t> &templ,
        EyePair &eyeCoordinates) = 0;

    /**
     * @brief This function compares two proprietary templates and outputs a
     * similarity score, which need not satisfy the metric properties. When
     * either or both of the input templates are the result of a failed
     * template generation, the similarity score shall be -1 and the function
     * return value shall be VerifTemplateError.
     *
     * param[in] verifTemplate
     * A verification template from createTemplate(role=Verification_11).
     * The underlying data can be accessed via verifTemplate.data().  The size,
     * in bytes, of the template could be retrieved as verifTemplate.size().
     * param[in] enrollTemplate
     * An enrollment template from createTemplate(role=Enrollment_11).
     * The underlying data can be accessed via enrollTemplate.data().  The size,
     * in bytes, of the template could be retrieved as enrollTemplate.size().
     * param[out] similarity
     * A similarity score resulting from comparison of the templates,
     * on the range [0,DBL_MAX].
     *
     */
    virtual ReturnStatus
    matchTemplates(
        const std::vector<uint8_t> &verifTemplate,
        const std::vector<uint8_t> &enrollTemplate,
        double &similarity) = 0;

    /**
     * @brief This function sets the GPU device number to be used by all
     * subsequent implementation function calls.  gpuNum is a zero-based
     * sequence value of which GPU device to use.  0 would mean the first
     * detected GPU, 1 would be the second GPU, etc.  If the implementation
     * does not use GPUs, then this function call should simply do nothing.
     *
     * @param[in] gpuNum
     * Index number representing which GPU to use
     */
    virtual ReturnStatus
    setGPU(uint8_t gpuNum) = 0;

    /**
     * @brief
     * Factory method to return a managed pointer to the VerifInterface object.
     * @details
     * This function is implemented by the submitted library and must return
     * a managed pointer to the VerifInterface object.
     *
     * @note
     * A possible implementation might be:
     * return (std::make_shared<Implementation>());
     */
    static std::shared_ptr<VerifInterface>
    getImplementation();
};
/* End of VerifInterface */
}

#endif /* FRPC_H_ */

