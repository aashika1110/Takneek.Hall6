// This file was generated by generate-classes.
// DO NOT EDIT THIS FILE!
#pragma once

#include "TextureInfoJsonHandler.h"

#include <CesiumGltf/FeatureIdTexture.h>
#include <CesiumJsonReader/ArrayJsonHandler.h>
#include <CesiumJsonReader/IntegerJsonHandler.h>

namespace CesiumJsonReader {
class JsonReaderOptions;
}

namespace CesiumGltfReader {
class FeatureIdTextureJsonHandler : public TextureInfoJsonHandler {
public:
  using ValueType = CesiumGltf::FeatureIdTexture;

  FeatureIdTextureJsonHandler(
      const CesiumJsonReader::JsonReaderOptions& options) noexcept;
  void
  reset(IJsonHandler* pParentHandler, CesiumGltf::FeatureIdTexture* pObject);

  virtual IJsonHandler* readObjectKey(const std::string_view& str) override;

protected:
  IJsonHandler* readObjectKeyFeatureIdTexture(
      const std::string& objectType,
      const std::string_view& str,
      CesiumGltf::FeatureIdTexture& o);

private:
  CesiumGltf::FeatureIdTexture* _pObject = nullptr;
  CesiumJsonReader::
      ArrayJsonHandler<int64_t, CesiumJsonReader::IntegerJsonHandler<int64_t>>
          _channels;
};
} // namespace CesiumGltfReader
