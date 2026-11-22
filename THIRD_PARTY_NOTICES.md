# Third-party notices

## ALVR (streaming transport), approved for adoption

| Field | Value |
|---|---|
| Project | ALVR, PC VR streaming to standalone headsets |
| Source | https://github.com/alvr-org/ALVR |
| Version | v20.14.1 (released 2025-07-14) |
| Commit | `a9f6542fa507a841f40ab4f3fcb531427cd02550` |
| License | MIT |
| Copyright | Copyright (c) 2018-2019 polygraphene; Copyright (c) 2020-2024 alvr-org |
| License text | `third_party/licenses/alvr-LICENSE.txt` |
| Used for | Headset-side streaming client (client_openxr, client_core, graphics, sockets, packets) |
| Integration | Not yet integrated. Planned: pin the commit above, keep upstream code unmodified, and store any change as a separate patch file under `third_party/patches/`. |

Status: the owner approved ALVR as the streaming transport. The tag v20.14.1 is the latest tagged release; the upstream repo has newer commits, so reconfirm the tag before the first integration.

Note: the latest tag is from July 2025 while the upstream repo is actively updated. Check whether a newer tag exists before release.
