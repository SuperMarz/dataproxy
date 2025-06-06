// Copyright 2024 Ant Group Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "dataproxy_sdk/data_proxy_pb.h"

#include <unordered_map>

#include "gtest/gtest.h"

#include "proto/data_proxy_pb.pb.h"

namespace dataproxy_sdk {

TEST(DataProxyPD, GetConfigFromEnv) {
  std::unordered_map<std::string, std::string> env_values = {
      {"CLIENT_CERT_FILE", "aaa"},
      {"CLIENT_PRIVATE_KEY_FILE", "bbb"},
      {"TRUSTED_CA_FILE", "ccc"},
      {"KUSCIA_DATA_MESH_ADDR", "ddd"}};
  for (const auto& it : env_values) {
    setenv(it.first.c_str(), it.second.c_str(), 0);
  }

  proto::DataProxyConfig config;
  GetDPConfigValueFromEnv(&config);

  for (const auto& it : env_values) {
    unsetenv(it.first.c_str());
  }

  std::cout << config.DebugString() << std::endl;

  EXPECT_EQ(config.tls_config().certificate_path(),
            env_values["CLIENT_CERT_FILE"]);
  EXPECT_EQ(config.tls_config().private_key_path(),
            env_values["CLIENT_PRIVATE_KEY_FILE"]);
  EXPECT_EQ(config.tls_config().ca_file_path(), env_values["TRUSTED_CA_FILE"]);
  EXPECT_EQ(config.data_proxy_addr(), env_values["KUSCIA_DATA_MESH_ADDR"]);
}

TEST(DataProxyPD, GetConfig) {
  static const std::string kCertificate = "eee";
  static const std::string kPrivateKey = "fff";
  static const std::string kCa = "ggg";
  static const std::string kAddress = "hhh";

  proto::DataProxyConfig config;
  config.mutable_tls_config()->set_certificate_path(kCertificate);
  config.mutable_tls_config()->set_private_key_path(kPrivateKey);
  config.mutable_tls_config()->set_ca_file_path(kCa);
  config.set_data_proxy_addr(kAddress);

  GetDPConfigValueFromEnv(&config);

  std::cout << config.DebugString() << std::endl;

  EXPECT_EQ(config.tls_config().certificate_path(), kCertificate);
  EXPECT_EQ(config.tls_config().private_key_path(), kPrivateKey);
  EXPECT_EQ(config.tls_config().ca_file_path(), kCa);
  EXPECT_EQ(config.data_proxy_addr(), kAddress);
}

TEST(DataProxyPD, GetConfigWithNullEnv) {
  static const std::string kCertificate = "iii";
  static const std::string kPrivateKey = "jjj";
  static const std::string kCa = "kkk";
  static const std::string kAddress = "lll";

  std::unordered_map<std::string, std::string> env_values = {
      {"CLIENT_CERT_FILE", ""},
      {"CLIENT_PRIVATE_KEY_FILE", ""},
      {"TRUSTED_CA_FILE", ""},
      {"KUSCIA_DATA_MESH_ADDR", ""}};
  for (const auto& it : env_values) {
    setenv(it.first.c_str(), it.second.c_str(), 0);
  }

  proto::DataProxyConfig config;
  config.mutable_tls_config()->set_certificate_path(kCertificate);
  config.mutable_tls_config()->set_private_key_path(kPrivateKey);
  config.mutable_tls_config()->set_ca_file_path(kCa);
  config.set_data_proxy_addr(kAddress);

  GetDPConfigValueFromEnv(&config);

  for (const auto& it : env_values) {
    unsetenv(it.first.c_str());
  }

  std::cout << config.DebugString() << std::endl;

  EXPECT_EQ(config.tls_config().certificate_path(), kCertificate);
  EXPECT_EQ(config.tls_config().private_key_path(), kPrivateKey);
  EXPECT_EQ(config.tls_config().ca_file_path(), kCa);
  EXPECT_EQ(config.data_proxy_addr(), kAddress);
}

TEST(DataProxyPD, GetConfigWithEnv) {
  static const std::string kCertificate = "mmm";
  static const std::string kPrivateKey = "nnn";
  static const std::string kCa = "ooo";
  static const std::string kAddress = "ppp";

  std::unordered_map<std::string, std::string> env_values = {
      {"CLIENT_CERT_FILE", "qqq"},
      {"CLIENT_PRIVATE_KEY_FILE", "rrr"},
      {"TRUSTED_CA_FILE", "sss"},
      {"KUSCIA_DATA_MESH_ADDR", "ttt"}};
  for (const auto& it : env_values) {
    setenv(it.first.c_str(), it.second.c_str(), 0);
  }

  proto::DataProxyConfig config;
  config.mutable_tls_config()->set_certificate_path(kCertificate);
  config.mutable_tls_config()->set_private_key_path(kPrivateKey);
  config.mutable_tls_config()->set_ca_file_path(kCa);
  config.set_data_proxy_addr(kAddress);

  GetDPConfigValueFromEnv(&config);

  for (const auto& it : env_values) {
    unsetenv(it.first.c_str());
  }

  std::cout << config.DebugString() << std::endl;

  EXPECT_EQ(config.tls_config().certificate_path(),
            env_values["CLIENT_CERT_FILE"]);
  EXPECT_EQ(config.tls_config().private_key_path(),
            env_values["CLIENT_PRIVATE_KEY_FILE"]);
  EXPECT_EQ(config.tls_config().ca_file_path(), env_values["TRUSTED_CA_FILE"]);
  EXPECT_EQ(config.data_proxy_addr(), env_values["KUSCIA_DATA_MESH_ADDR"]);
}

TEST(DataProxyPD, GetConfigDefault) {
  proto::DataProxyConfig config;

  GetDPConfigValueFromEnv(&config);

  std::cout << config.DebugString() << std::endl;

  EXPECT_EQ(config.tls_config().certificate_path(), "");
  EXPECT_EQ(config.tls_config().private_key_path(), "");
  EXPECT_EQ(config.tls_config().ca_file_path(), "");
  EXPECT_EQ(config.data_proxy_addr(), "datamesh:8071");
}

TEST(DataProxyPD, PbToProto) {
  static const std::string kDomainDataId = "123";
  static const std::string kAttributeType = "456";
  static const std::string kAttributeKey = "789";
  static const std::string kColumnName = "name";

  pb::UploadInfo info;
  info.set_domaindata_id(kDomainDataId);
  info.mutable_attributes()->insert({kAttributeType, kAttributeKey});
  auto* column = info.add_columns();
  column->set_name("name");

  proto::UploadInfo protoInfo;
  protoInfo.set_domaindata_id(info.domaindata_id());
  for (const auto& it : info.attributes()) {
    protoInfo.mutable_attributes()->insert({it.first, it.second});
  }
  for (const auto& it : info.columns()) {
    auto* protoColumn = protoInfo.add_columns();
    protoColumn->set_name(it.name());
  }

  EXPECT_EQ(protoInfo.domaindata_id(), kDomainDataId);
  auto it = protoInfo.mutable_attributes()->find(kAttributeType);
  EXPECT_TRUE(it != protoInfo.mutable_attributes()->end());
  EXPECT_EQ(it->second, kAttributeKey);
  EXPECT_TRUE(protoInfo.columns_size() == info.columns_size());
  EXPECT_EQ(protoInfo.columns(0).name(), kColumnName);
}

}  // namespace dataproxy_sdk
