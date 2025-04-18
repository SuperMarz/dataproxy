# Copyright 2024 Ant Group Co., Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
warpper bazel cc_xx to modify flags.
"""

load("@rules_cc//cc:defs.bzl", "cc_library", "cc_test")

WARNING_FLAGS = [
    "-Wall",
    "-Wextra",
    "-Werror",
    "-Wno-unused-parameter",
    "-Wnon-virtual-dtor",
] + select({
    "@bazel_tools//src/conditions:darwin": ["-Wunused-const-variable"],
    "//conditions:default": ["-Wunused-const-variable=1"],
})
DEBUG_FLAGS = ["-O0", "-g"]
RELEASE_FLAGS = ["-O2"]
FAST_FLAGS = ["-O1"]

def _dataproxy_copts():
    return select({
        "//:dataproxy_build_as_release": RELEASE_FLAGS,
        "//:dataproxy_build_as_debug": DEBUG_FLAGS,
        "//:dataproxy_build_as_fast": FAST_FLAGS,
        "//conditions:default": FAST_FLAGS,
    }) + WARNING_FLAGS

def dataproxy_cc_library(
        linkopts = [],
        copts = [],
        deps = [],
        **kargs):
    cc_library(
        linkopts = linkopts,
        copts = _dataproxy_copts() + copts,
        deps = deps + [
            "@spdlog//:spdlog",
        ],
        **kargs
    )

def dataproxy_cc_test(
        linkopts = [],
        copts = [],
        deps = [],
        **kwargs):
    cc_test(
        linkopts = linkopts,
        copts = _dataproxy_copts() + copts,
        deps = [
            # use tcmalloc same as release bins. make them has same behavior on mem.
            "@googletest//:gtest_main",
        ] + deps,
        **kwargs
    )
