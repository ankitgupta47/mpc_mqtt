load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")
http_archive(
  name = "com_google_googletest",
  urls = ["https://github.com/google/googletest/archive/a1cd5f01ca64e9ae58e4ff8dcf5d1b0e86360f72.zip"],
  strip_prefix = "googletest-a1cd5f01ca64e9ae58e4ff8dcf5d1b0e86360f72", #googletest-commitid
)


http_archive(
    name = "rules_foreign_cc",
    sha256 = "bcd0c5f46a49b85b384906daae41d277b3dc0ff27c7c752cc51e43048a58ec83",
    strip_prefix = "rules_foreign_cc-0.7.1",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/0.7.1.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

# This sets up some common toolchains for building targets. For more details, please see
# https://bazelbuild.github.io/rules_foreign_cc/0.7.1/flatten.html#rules_foreign_cc_dependencies
rules_foreign_cc_dependencies()


new_git_repository(
    name = "paho_mqtt_c",
    build_file_content = """filegroup(name = "mqtt_c", srcs = glob(["src/*"]), visibility = ["//visibility:public"])""",
    #strip_prefix = "Eclipse-Paho-MQTT-C-1.3.10-Linux",
    remote = "https://github.com/eclipse/paho.mqtt.c",
    commit = "556cd568345e47b70da603edc92f11ff94a6161f",
    init_submodules =1,
)

new_git_repository(
    name = "paho_mqtt_cpp",
    build_file_content = """filegroup(name = "mqtt_cpp", srcs = glob(["src/**"]), visibility = ["//visibility:public"],)""",
    #strip_prefix = "paho.mqtt.cpp-1.2.0",
    remote = "https://github.com/eclipse/paho.mqtt.cpp",
    commit = "2ff3d155dcd10564f1816675789284b4efd79eb7",
)



