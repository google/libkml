// Copyright 2008, Google Inc. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, 
//     this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//  3. Neither the name of Google Inc. nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// This file contains the unit tests for the XsdFile class.

#include "kml/xsd/xsd_file.h"
#include "boost/scoped_ptr.hpp"
#include "kml/base/attributes.h"
#include "kml/base/unit_test.h"
#include "kml/xsd/xsd_util.h"

using kmlbase::Attributes;

namespace kmlxsd {

// This class is the unit test fixture for the XsdFile class.
class XsdFileTest : public CPPUNIT_NS::TestFixture {
  CPPUNIT_TEST_SUITE(XsdFileTest);
  CPPUNIT_TEST(TestConstructor);
  CPPUNIT_TEST(TestCreateFromParse);
  CPPUNIT_TEST(TestSetSchema);
  CPPUNIT_TEST(TestFindElementType);
  CPPUNIT_TEST(TestAddComplexType);
  CPPUNIT_TEST(TestResolveRef);
  CPPUNIT_TEST(TestGetTypeHierarchy);
  CPPUNIT_TEST(TestFindElementTypeByName);
  CPPUNIT_TEST(TestGetAbstractElements);
  CPPUNIT_TEST(TestGetComplexElements);
  CPPUNIT_TEST(TestGetSimpleElements);
  CPPUNIT_TEST(TestSetGetAlias);
  CPPUNIT_TEST(TestGetAllTypes);
  CPPUNIT_TEST(TestFindChildElements);
  CPPUNIT_TEST_SUITE_END();

 public:
  void setUp() {
    xsd_file_.reset(new XsdFile);
  }

 protected:
  void TestConstructor();
  void TestCreateFromParse();
  void TestSetSchema();
  void TestFindElementType();
  void TestAddComplexType();
  void TestResolveRef();
  void TestGetTypeHierarchy();
  void TestFindElementTypeByName();
  void TestGetAbstractElements();
  void TestGetComplexElements();
  void TestGetSimpleElements();
  void TestSetGetAlias();
  void TestGetAllTypes();
  void TestFindChildElements();

 private:
  void SetTestSchema();
  void AddTestComplexTypes();
  void AddTestElements();
  boost::scoped_ptr<XsdFile> xsd_file_;
};

CPPUNIT_TEST_SUITE_REGISTRATION(XsdFileTest);

void XsdFileTest::TestConstructor() {
  CPPUNIT_ASSERT(xsd_file_.get());
}

// Verify CreateFromParse().
void XsdFileTest::TestCreateFromParse() {
  // Verify known bad XSD returns NULL.
  std::string errors;
  CPPUNIT_ASSERT(!XsdFile::CreateFromParse("not xsd", &errors));
  CPPUNIT_ASSERT(!errors.empty());

  // TODO: more
}

// Verify set_schema(),
void XsdFileTest::TestSetSchema() {
  const std::string kPrefix("myml");
  const std::string kNamespace("my:own:namespace");
  const XsdSchemaPtr xsd_schema = CreateXsdSchema(kPrefix, kNamespace);

  // Call the method under test
  xsd_file_->set_schema(xsd_schema);

  // Setting the <xs:schema> sets the namespace and prefix for this file.
  CPPUNIT_ASSERT_EQUAL(kPrefix, xsd_file_->get_target_namespace_prefix());
  CPPUNIT_ASSERT_EQUAL(kNamespace, xsd_file_->get_target_namespace());
}

// Verify add_type(), FindElement(), and FindElementType().
void XsdFileTest::TestFindElementTypeByName() {
  // Create the <xs:schema ... >
  const std::string kPrefix("myml");
  const std::string kNamespace("my:own:namespace");
  xsd_file_->set_schema(CreateXsdSchema(kPrefix, kNamespace));
  // Create an <xs:complexType name="MyCoolType"/>
  const std::string kMyType("MyCoolType");
  xsd_file_->add_type(CreateXsdComplexType(kMyType));
  const std::string kMyElement("MyCoolness");
  // <xs:element name="MyCoolness" type="myml:MyCoolType"/>
  xsd_file_->add_element(CreateXsdElement(kMyElement, kPrefix + ":" + kMyType));

  const XsdElementPtr element = xsd_file_->FindElement(kMyElement);
  CPPUNIT_ASSERT(element);
  const XsdTypePtr complex_type = xsd_file_->FindElementType(element);
  CPPUNIT_ASSERT(complex_type);
  CPPUNIT_ASSERT_EQUAL(kMyType, complex_type->get_name());
}

// Verify add_type() and GetChildElements().
void XsdFileTest::TestAddComplexType() {
  // This is an instance document this XSD-let describes:
  // <MyCoolness>
  //   <lon>-120.123</lon>
  //   <lat>37.37</lon>
  // </MyCoolness>

  // This is the XSD for the type of MyCoolness:
  // <xs:complexType name="MyCoolType">
  //   <complexContent>
  //     <sequence>
  //       <element name="lon" type="double"/>
  //       <element name="lat" type="double"/>
  //     </sequence>
  //   </complexContent>
  // </xs:complexType">
  const std:: string kMyElement("MyCoolness");
  const std:: string kMyType("MyCoolType");
  const std::string kLon("lon");
  const std::string kLat("lat");
  // Create the <xs:schema ... >
  const std::string kPrefix("myml");
  const std::string kNamespace("my:own:namespace");
  xsd_file_->set_schema(CreateXsdSchema(kPrefix, kNamespace));
  XsdComplexTypePtr xsd_complex_type = CreateXsdComplexType(kMyType);
  xsd_complex_type->add_element(CreateXsdElement(kLon, "double"));
  xsd_complex_type->add_element(CreateXsdElement(kLat, "double"));
  xsd_file_->add_type(xsd_complex_type);

  // <xs:element name="MyCoolness" type="MyCoolType"/>
  xsd_file_->add_element(CreateXsdElement(kMyElement, kPrefix + ":" + kMyType));

  XsdElementVector child_elements;
  xsd_file_->GetChildElements(kMyElement, &child_elements);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), child_elements.size());
  CPPUNIT_ASSERT_EQUAL(kLon, child_elements[0]->get_name());
  CPPUNIT_ASSERT_EQUAL(kLat, child_elements[1]->get_name());
}

// Verify ResolveRef.
void XsdFileTest::TestResolveRef() {
  const std::string kPrefix("myml");
  const std::string kNamespace("my:own:namespace");
  const std::string kMyElement("MyCoolness");

  // Verify failure if no XsdSchema set for this XsdFile.
  CPPUNIT_ASSERT(!xsd_file_->ResolveRef(kPrefix + ":" + kMyElement));

  // Build enough of an XsdFile to successfully use ResolveRef().
  // Create the <xs:schema ... >
  xsd_file_->set_schema(CreateXsdSchema(kPrefix, kNamespace));
  // Create a global <xs:element name="myCoolness" type="string"/>
  xsd_file_->add_element(CreateXsdElement(kMyElement, "string"));

  // Call the method under test for a good case.
  XsdElementPtr element = xsd_file_->ResolveRef(kPrefix + ":" + kMyElement);
  CPPUNIT_ASSERT(element);
  CPPUNIT_ASSERT_EQUAL(kMyElement, element->get_name());

  // Call the method under test for some failure cases.
  CPPUNIT_ASSERT(!xsd_file_->ResolveRef("nosuchprefix:" + kMyElement));
  CPPUNIT_ASSERT(!xsd_file_->ResolveRef(kPrefix + ":nosuchelement"));
}

static const char* kTestPrefix = "myml";
static const char* kTestTargetNamespace = "my:own:namespace";

static const struct {
  const char* type_name;
  const char* extension_base;  // NULL if no base type.
} kTestComplexTypes[] = {
  { "FeatureType", "myml:ObjectType" },
  { "GeometryType", "myml:ObjectType" },
  { "LineStringType", "myml:GeometryType" },
  { "ObjectType", NULL },
  { "PlacemarkType", "myml:FeatureType" },
  { "PointType", "myml:GeometryType" }
};

static const struct {
  const char* name;
  const char* type;
  const char* abstract;
} kTestElements[] = {
  { "altitude", "double", "false" },
  { "altitudeMode", "myml:altitudeModeEnum", "false" },
  { "name", "string", "false" },
  { "visibility", "boolean", "false" },
  { "FeatureGroup", "myml:FeatureType", "true" },
  { "GeometryGroup", "myml:GeometryType", "true" },
  { "LineString", "myml:LineStringType", "false" },
  { "ObjectGroup", "myml:ObjectType", "true" },
  { "Placemark", "myml:PlacemarkType", "false" },
  { "Point", "myml:PointType", "false" }
};

void XsdFileTest::SetTestSchema() {
  // Build enough of an XsdFile to successfull use ResolveRef(), and
  // GetTypeHierarchy().
  // Create the <xs:schema ... >
  xsd_file_->set_schema(CreateXsdSchema(kTestPrefix, kTestTargetNamespace));
}

// This is an internal utility to add the kTestComplexTypes entries to the
// complex types map in the kml_file_.
void XsdFileTest::AddTestComplexTypes() {
  // Create some <xs:complexTypes> which extend each other.
  size_t size = sizeof(kTestComplexTypes)/sizeof(kTestComplexTypes[0]);
  for (size_t i = 0; i < size; ++i) {
    XsdComplexTypePtr complex_type =
        CreateXsdComplexType(kTestComplexTypes[i].type_name);
    if (kTestComplexTypes[i].extension_base) {
      complex_type->set_extension_base(kTestComplexTypes[i].extension_base);
    }
    xsd_file_->add_type(complex_type);
  }
}

void XsdFileTest::AddTestElements() {
  size_t size = sizeof(kTestElements)/sizeof(kTestElements[0]);
  for (size_t i = 0; i < size; ++i) {
    Attributes attributes;
    attributes.SetString(kName, kTestElements[i].name);
    attributes.SetString(kType, kTestElements[i].type);
    attributes.SetString(kAbstract, kTestElements[i].abstract);
    XsdElementPtr element = XsdElement::Create(attributes);
    CPPUNIT_ASSERT(element);
    xsd_file_->add_element(element);
  }
}

// Verify GetTypeHierarchy().
void XsdFileTest::TestGetTypeHierarchy() {
  SetTestSchema();
  AddTestComplexTypes();
  AddTestElements();

  std::vector<XsdComplexTypePtr> hier;
  const XsdElementPtr element = xsd_file_->FindElement("Placemark");
  XsdTypePtr derived = xsd_file_->FindElementType(element);
  CPPUNIT_ASSERT(derived);
  XsdComplexTypePtr complex_type = XsdComplexType::AsComplexType(derived);
  CPPUNIT_ASSERT(complex_type);
  CPPUNIT_ASSERT(xsd_file_->GetTypeHierarchy(complex_type, &hier));
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), hier.size());
  CPPUNIT_ASSERT_EQUAL(std::string("FeatureType"), hier[0]->get_name());
  CPPUNIT_ASSERT_EQUAL(std::string("ObjectType"), hier[1]->get_name());
}

// XXX Verify FindElementType().  ByName() we do above...
void XsdFileTest::TestFindElementType() {
#if 0
  SetTestSchema();
  AddTestComplexTypes();
  AddTestElements();
  CPPUNIT_ASSERT_EQUAL(std::string("PlacemarkType"),
                       xsd_file_->FindElementTypeByName("Placemark"));
  CPPUNIT_ASSERT_EQUAL(std::string("PointType"),
                       xsd_file_->FindElementTypeByName("Point"));
#endif
}

// Verify GetAbstractElements().
void XsdFileTest::TestGetAbstractElements() {
  XsdElementVector elements;
  xsd_file_->GetAbstractElements(&elements);
  // Empty XsdFile, no elements, no abstract elements, no nuthin.
  CPPUNIT_ASSERT(elements.empty());

  // Add the test elements to the XsdFile.
  AddTestElements();
  xsd_file_->GetAbstractElements(&elements);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), elements.size());
  CPPUNIT_ASSERT_EQUAL(std::string("FeatureGroup"), elements[0]->get_name());
  CPPUNIT_ASSERT_EQUAL(std::string("GeometryGroup"), elements[1]->get_name());
  CPPUNIT_ASSERT_EQUAL(std::string("ObjectGroup"), elements[2]->get_name());

}

// Verify GetComplexElements().
void XsdFileTest::TestGetComplexElements() {
  XsdElementVector element_names;
  xsd_file_->GetComplexElements(&element_names);
  // Empty XsdFile, no abstract elements.
  CPPUNIT_ASSERT(element_names.empty());

  // Add the test namespace, types, and elements to the XsdFile.
  SetTestSchema();
  AddTestComplexTypes();
  AddTestElements();
  xsd_file_->GetComplexElements(&element_names);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), element_names.size());
  CPPUNIT_ASSERT_EQUAL(std::string("LineString"), element_names[0]->get_name());
  CPPUNIT_ASSERT_EQUAL(std::string("Placemark"), element_names[1]->get_name());
  CPPUNIT_ASSERT_EQUAL(std::string("Point"), element_names[2]->get_name());
}

// Verify GetSimpleElements().
void XsdFileTest::TestGetSimpleElements() {
  XsdElementVector element_names;
  xsd_file_->GetSimpleElements(&element_names);
  // Empty XsdFile, no simple elements.
  CPPUNIT_ASSERT(element_names.empty());

  // Add the test namespace, types, and elements to the XsdFile.
  SetTestSchema();
  AddTestComplexTypes();
  AddTestElements();
  xsd_file_->GetSimpleElements(&element_names);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(3), element_names.size());
}

// Verify TestSetGetAlias();
void XsdFileTest::TestSetGetAlias() {
  const std::string kFeature("Feature");
  const std::string kAbstractFeatureGroup("AbstractFeatureGroup");
  const std::string kGeometry("Geometry");
  const std::string kAbstractGeometryGroup("AbstractGeometryGroup");

  // An empty XsdFile has no aliases:
  CPPUNIT_ASSERT_EQUAL(std::string(""),
                       xsd_file_->get_alias(kAbstractFeatureGroup));

  // Add aliases using set_alias().
  xsd_file_->set_alias(kAbstractFeatureGroup, kFeature);
  xsd_file_->set_alias(kAbstractGeometryGroup, kGeometry);

  // Verify the proper result for get_alias().
  CPPUNIT_ASSERT_EQUAL(kFeature, xsd_file_->get_alias(kAbstractFeatureGroup));
  CPPUNIT_ASSERT_EQUAL(kGeometry, xsd_file_->get_alias(kAbstractGeometryGroup));
}

// Verify GetAllTypes().
void XsdFileTest::TestGetAllTypes() {
  // Add the test namespace and types the XsdFile.
  SetTestSchema();
  AddTestComplexTypes();

  XsdTypeVector types;
  xsd_file_->GetAllTypes(&types);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(6), types.size());
}

// Verify TestFindChildElements().
void XsdFileTest::TestFindChildElements() {
  // Verify NULL conditions: NULL complex_type and nothing in XsdFile.
  XsdComplexTypePtr complex_type;
  XsdElementVector children;
  xsd_file_->FindChildElements(complex_type, &children);
  CPPUNIT_ASSERT(children.empty());

  // Add the test namespace and types the XsdFile.
  SetTestSchema();
  AddTestComplexTypes();
  xsd_file_->FindChildElements(complex_type, &children);
  CPPUNIT_ASSERT(children.empty());

  // Look for the given type.
  const std::string kPlacemarkType("PlacemarkType");
  complex_type = XsdComplexType::AsComplexType(
      xsd_file_->FindType(kPlacemarkType));
  CPPUNIT_ASSERT(complex_type);
  CPPUNIT_ASSERT_EQUAL(kPlacemarkType, complex_type->get_name());

  // This has no children.
  xsd_file_->FindChildElements(complex_type, &children);
  CPPUNIT_ASSERT(children.empty());

  // Give it some children.
  const std::string kLatitude("latitude");
  const std::string kLongitude("longitude");
  const std::string kDouble("double");
  complex_type->add_element(CreateXsdElement(kLatitude, kDouble));
  complex_type->add_element(CreateXsdElement(kLongitude, kDouble));
  
  // This now has some children.
  xsd_file_->FindChildElements(complex_type, &children);
  CPPUNIT_ASSERT_EQUAL(static_cast<size_t>(2), children.size());
  CPPUNIT_ASSERT_EQUAL(kLatitude, children[0]->get_name());
  CPPUNIT_ASSERT_EQUAL(kLongitude, children[1]->get_name());
}

}  // end namespace kmlxsd

TEST_MAIN
