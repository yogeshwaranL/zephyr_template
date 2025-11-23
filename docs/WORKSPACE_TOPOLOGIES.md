# Zephyr Workspace Topologies - Visual Guide

This guide provides visual diagrams and explanations of Zephyr's workspace topologies (T1, T2, T3) and the concept of manifest repositories.

## Table of Contents

1. [What is a Manifest Repository?](#what-is-a-manifest-repository)
2. [T1: Star Topology (Zephyr-centric)](#t1-star-topology-zephyr-centric)
3. [T2: Star Topology (Application-centric)](#t2-star-topology-application-centric)
4. [T3: Forest Topology](#t3-forest-topology)
5. [Side-by-Side Comparison](#side-by-side-comparison)
6. [How West Uses Manifest Repositories](#how-west-uses-manifest-repositories)

---

## What is a Manifest Repository?

A **manifest repository** is the repository that contains the `west.yml` file, which defines:

1. **What projects to fetch** (Zephyr, HALs, modules, etc.)
2. **Where to fetch them from** (Git URLs)
3. **Which versions to use** (branches, tags, commits)
4. **Where to place each project** in the workspace

Think of it like a **shopping list + assembly instructions** for your development environment.

### Manifest Repository Anatomy

```yaml
# west.yml - The Manifest File
manifest:
  version: "0.13"

  # WHO AM I? (manifest repository identity)
  self:
    path: .   # Where THIS repository should be placed

  # REMOTE SOURCES (where to download from)
  remotes:
    - name: zephyrproject-rtos
      url-base: https://github.com/zephyrproject-rtos

  # WHAT TO FETCH (dependencies)
  projects:
    - name: zephyr              # Project name
      remote: zephyrproject-rtos  # Where to get it
      revision: v3.7.0          # Which version
      import: true              # Import its dependencies too
```

### Key Concept: The Manifest Repository is "Mission Control"

```
┌─────────────────────────────────────────────────────────┐
│         MANIFEST REPOSITORY (Mission Control)           │
│                                                          │
│  ┌────────────────────────────────────────────────┐    │
│  │            west.yml (The Manifest)              │    │
│  │                                                 │    │
│  │  "Fetch Zephyr v3.7.0 from GitHub"            │    │
│  │  "Fetch HAL Nordic from GitHub"               │    │
│  │  "Fetch mbedTLS from GitHub"                  │    │
│  │  "Place everything in this structure..."      │    │
│  └────────────────────────────────────────────────┘    │
│                                                          │
│              ⬇ west update ⬇                            │
└─────────────────────────────────────────────────────────┘
                       │
       ┌───────────────┼───────────────┐
       ▼               ▼               ▼
   ┌────────┐    ┌─────────┐    ┌──────────┐
   │ Zephyr │    │   HALs  │    │ Modules  │
   │ v3.7.0 │    │ (Nordic)│    │(mbedTLS) │
   └────────┘    └─────────┘    └──────────┘
```

**The manifest repository decides:**
- ✅ Which projects exist in the workspace
- ✅ What versions are used
- ✅ Where they are placed
- ✅ How they relate to each other

---

## T1: Star Topology (Zephyr-centric)

**Manifest Repository:** `zephyr/` (Zephyr itself is mission control)

### Directory Structure

```
workspace-root/
│
├── .west/                   # West configuration
│   └── config               # Points to zephyr/ as manifest
│
├── zephyr/                  # ⭐ MANIFEST REPOSITORY ⭐
│   ├── west.yml             # The manifest (Zephyr defines everything)
│   ├── kernel/
│   ├── drivers/
│   ├── samples/
│   └── ... (Zephyr source code)
│
├── modules/
│   ├── hal/
│   │   ├── nordic/
│   │   ├── stm32/
│   │   └── espressif/
│   ├── crypto/
│   │   └── mbedtls/
│   └── ...
│
├── tools/
│
└── my-application/          # Your app (just another folder)
    ├── CMakeLists.txt
    ├── prj.conf
    └── src/
        └── main.c
```

### Visual Flow

```
┌──────────────────────────────────────────────────────────────┐
│                    T1: Zephyr is the Boss                    │
└──────────────────────────────────────────────────────────────┘

      Step 1: Clone Zephyr
      ▼
┌─────────────────┐
│  zephyr/        │  ← Manifest Repository
│  ├── west.yml   │     (Contains instructions)
│  └── kernel/    │
└─────────────────┘

      Step 2: Run 'west update'
      ▼
      Zephyr's west.yml says:
      "Fetch my dependencies (HALs, modules)"
      ▼
┌─────────────────┐
│  workspace/     │
│  ├── zephyr/    │  ← Manifest repo (defines everything)
│  ├── modules/   │  ← Fetched by Zephyr
│  └── tools/     │  ← Fetched by Zephyr
└─────────────────┘

      Step 3: Add your application
      ▼
┌─────────────────┐
│  workspace/     │
│  ├── zephyr/    │  ← The boss
│  ├── modules/   │
│  ├── tools/     │
│  └── my-app/    │  ← Your app (not special)
└─────────────────┘
```

### Pros & Cons

✅ **Pros:**
- Easy to get started (official Zephyr setup)
- Direct access to all Zephyr samples
- Good for contributing to Zephyr

❌ **Cons:**
- Your app is mixed with Zephyr source
- Hard to version control just your app
- Not suitable for production projects

---

## T2: Star Topology (Application-centric)

**Manifest Repository:** Your application repository (YOU are mission control)

### Directory Structure

```
my-application/              # ⭐ MANIFEST REPOSITORY ⭐
│                            # (Your app IS the manifest repo)
├── .git/                    # Your version control
├── .west/                   # West configuration
│   └── config               # Points to "." as manifest
│
├── west.yml                 # The manifest (YOU define everything)
│                            # ↑ This makes it the manifest repo
├── CMakeLists.txt           # Your application build
├── prj.conf                 # Your application config
├── Kconfig
│
├── src/                     # Your application code
│   └── main.c
│
├── boards/                  # Your board configs
│
├── docs/                    # Your documentation
│
├── zephyr/                  # Fetched by west (dependency)
│   ├── kernel/
│   ├── drivers/
│   └── ...
│
└── modules/                 # Fetched by west (dependency)
    ├── hal/
    └── crypto/
```

### Visual Flow

```
┌──────────────────────────────────────────────────────────────┐
│              T2: Your Application is the Boss                │
└──────────────────────────────────────────────────────────────┘

      Step 1: Clone your app
      ▼
┌─────────────────────┐
│  my-app/            │  ← Manifest Repository
│  ├── west.yml       │     (Your instructions)
│  ├── CMakeLists.txt │
│  └── src/           │
│      └── main.c     │
└─────────────────────┘

      Step 2: Run 'west init -l .'
      ▼
      West recognizes: "This directory (.) is the manifest"

      Step 3: Run 'west update'
      ▼
      Your west.yml says:
      "Fetch Zephyr v3.7.0"
      "Fetch these modules"
      ▼
┌─────────────────────┐
│  my-app/            │  ← Manifest repo (YOU define everything)
│  ├── west.yml       │
│  ├── src/           │
│  ├── zephyr/        │  ← Fetched by YOUR manifest
│  └── modules/       │  ← Fetched by YOUR manifest
└─────────────────────┘
```

### The Power of T2

```
┌────────────────────────────────────────────────────────────┐
│                   T2 Manifest Control                      │
└────────────────────────────────────────────────────────────┘

YOUR west.yml controls everything:

manifest:
  self:
    path: .    ← "I AM the manifest, I live at root (.)"

  projects:
    - name: zephyr
      revision: v3.7.0   ← YOU decide which Zephyr version

    - name: my-custom-driver
      url: https://github.com/me/driver
      revision: v1.0.0   ← YOU add custom dependencies


Result: Self-contained, reproducible application repository
```

### Pros & Cons

✅ **Pros:**
- Your app is the center of the universe
- Self-contained (clone, build, done)
- Version control only your code
- Reproducible builds (pin Zephyr version)
- **Industry standard for products**

❌ **Cons:**
- Only supports one application per repository
- Need separate repos for multiple apps

---

## T3: Forest Topology

**Manifest Repository:** Separate dedicated manifest repository (standalone mission control)

### Directory Structure

```
workspace-root/
│
├── .west/
│   └── config               # Points to "my-workspace/" as manifest
│
├── my-workspace/            # ⭐ MANIFEST REPOSITORY ⭐
│   ├── .git/                # Manifest repo version control
│   └── west.yml             # The manifest (defines EVERYTHING)
│                            # ↑ This is the ONLY job of this repo
│
├── sensor-app/              # Application 1
│   ├── .git/                # Has its own version control
│   ├── CMakeLists.txt
│   ├── prj.conf
│   └── src/
│
├── gateway-app/             # Application 2
│   ├── .git/                # Has its own version control
│   ├── CMakeLists.txt
│   └── src/
│
├── custom-drivers/          # Custom module
│   ├── .git/
│   └── drivers/
│
├── zephyr/                  # Fetched by west
│
└── modules/                 # Fetched by west
```

### Visual Flow

```
┌──────────────────────────────────────────────────────────────┐
│        T3: Dedicated Manifest Controls Everything            │
└──────────────────────────────────────────────────────────────┘

      Step 1: Clone manifest repository
      ▼
┌─────────────────────┐
│  my-workspace/      │  ← Manifest Repository
│  └── west.yml       │     (ONLY contains instructions)
└─────────────────────┘
      │
      west.yml content:
      ├─ "Fetch Zephyr v3.7.0"
      ├─ "Fetch sensor-app from github.com/me/sensor"
      ├─ "Fetch gateway-app from github.com/me/gateway"
      └─ "Fetch custom-drivers from github.com/me/drivers"

      Step 2: Run 'west update'
      ▼
      Manifest fetches EVERYTHING (apps + Zephyr + modules)
      ▼
┌─────────────────────┐
│  workspace/         │
│  ├── my-workspace/  │  ← Manifest repo (the orchestrator)
│  ├── sensor-app/    │  ← Fetched app #1
│  ├── gateway-app/   │  ← Fetched app #2
│  ├── custom-drivers/│  ← Fetched module
│  ├── zephyr/        │  ← Fetched dependency
│  └── modules/       │  ← Fetched dependencies
└─────────────────────┘
```

### T3 Manifest Repository Role

```
┌────────────────────────────────────────────────────────────┐
│             T3: Manifest is the Conductor                  │
└────────────────────────────────────────────────────────────┘

    ┌─────────────────────────┐
    │  my-workspace/west.yml  │  ← Manifest Repository
    │  (The Conductor)         │
    └───────────┬──────────────┘
                │
    ┌───────────┴───────────┐
    │  Orchestrates these:  │
    └───────────┬───────────┘
                │
        ┌───────┼───────┬───────┬──────────┐
        ▼       ▼       ▼       ▼          ▼
    ┌────┐  ┌────┐  ┌────┐  ┌────┐    ┌────┐
    │App1│  │App2│  │App3│  │Zeph│    │HALs│
    └────┘  └────┘  └────┘  └────┘    └────┘

manifest:
  self:
    path: my-workspace   ← "I am JUST the conductor"

  projects:
    - name: sensor-app        ← Fetch application 1
      url: github.com/me/sensor
      path: sensor-app

    - name: gateway-app       ← Fetch application 2
      url: github.com/me/gateway
      path: gateway-app

    - name: zephyr            ← Fetch Zephyr
      revision: v3.7.0
```

### Pros & Cons

✅ **Pros:**
- Multiple applications in one workspace
- Shared dependencies (one Zephyr for all apps)
- Centralized version control of dependencies
- Good for product families

❌ **Cons:**
- More complex setup
- Overkill for single application
- Need separate repos for apps and manifest

---

## Side-by-Side Comparison

```
┌──────────────────────────────────────────────────────────────────────────────┐
│                         TOPOLOGY COMPARISON                                  │
└──────────────────────────────────────────────────────────────────────────────┘

T1: Zephyr-centric           T2: App-centric              T3: Forest
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

workspace/                   my-app/                      workspace/
├── zephyr/ ⭐               ├── west.yml ⭐              ├── manifest/ ⭐
│   └── west.yml             ├── src/                    │   └── west.yml
├── modules/                 ├── zephyr/                 ├── app1/
├── tools/                   └── modules/                ├── app2/
└── my-app/                                              ├── zephyr/
                                                         └── modules/

Manifest: zephyr/            Manifest: my-app/           Manifest: manifest/
Boss: Zephyr                 Boss: Your App              Boss: Dedicated repo
Apps: Many (unmanaged)       Apps: One (integrated)      Apps: Many (managed)
Use: Learning Zephyr         Use: Production app         Use: Product family
```

### Directory Tree Comparison

```
T1 STRUCTURE                    T2 STRUCTURE                    T3 STRUCTURE
═══════════════                 ═══════════════                 ═══════════════

workspace/                      my-temp-sensor/                 iot-products/
│                               │                               │
├─ .west/config                 ├─ .west/config                 ├─ .west/config
│  manifest: zephyr             │  manifest: .                  │  manifest: manifest
│                               │                               │
├─ 📋 zephyr/                   ├─ 📋 west.yml                  ├─ 📋 manifest/
│  ├─ west.yml ←manifest        │  manifest ↑                   │  ├─ .git/
│  ├─ kernel/                   │                               │  └─ west.yml ←manifest
│  ├─ drivers/                  ├─ .git/                        │
│  └─ samples/                  │                               ├─ temp-sensor/
│                               ├─ CMakeLists.txt               │  ├─ .git/
├─ modules/                     ├─ prj.conf                     │  ├─ CMakeLists.txt
│  ├─ hal/                      ├─ src/                         │  └─ src/
│  └─ crypto/                   │  └─ main.c                    │
│                               │                               ├─ humidity-sensor/
├─ tools/                       ├─ 📁 zephyr/                   │  ├─ .git/
│                               │  ├─ kernel/                   │  └─ src/
└─ 📁 my-app/                   │  └─ drivers/                  │
   ├─ CMakeLists.txt            │                               ├─ 📁 zephyr/
   ├─ prj.conf                  └─ 📁 modules/                  │  ├─ kernel/
   └─ src/                         ├─ hal/                      │  └─ drivers/
      └─ main.c                    └─ crypto/                   │
                                                                └─ 📁 modules/

📋 = Manifest Repository        📋 = Manifest Repository        📋 = Manifest Repository
📁 = Regular Directory/Repo     📁 = Dependency (fetched)       📁 = Dependency (fetched)

Manifest owns: Nothing          Manifest owns: Everything       Manifest owns: Everything
Your control: App only          Your control: Everything        Your control: Apps + Manifest
Git repos: 1 (app)             Git repos: 1 (app+manifest)     Git repos: Many (separate)
```

---

## How West Uses Manifest Repositories

### The West Initialization Process

```
┌─────────────────────────────────────────────────────────────┐
│              How 'west init' Finds the Manifest             │
└─────────────────────────────────────────────────────────────┘

Command: west init -l /path/to/manifest-repo
                      ↑
                      └─ "Look in this directory for west.yml"

West looks for:
┌─────────────────────┐
│ /path/to/repo/      │
│ └── west.yml        │  ← Found it! This is the manifest repo
└─────────────────────┘

West reads west.yml:
┌─────────────────────────────────────────┐
│ manifest:                               │
│   self:                                 │
│     path: some-path  ← WHERE AM I?     │
│                                         │
│   projects:                             │
│     - name: zephyr   ← WHAT TO FETCH?  │
│     - name: modules  ← WHAT TO FETCH?  │
└─────────────────────────────────────────┘

West creates .west/config:
┌─────────────────────────────┐
│ [manifest]                  │
│ path = some-path            │
│                             │
│ "The manifest is at:        │
│  some-path/west.yml"        │
└─────────────────────────────┘
```

### West Update Process

```
┌─────────────────────────────────────────────────────────────┐
│                  How 'west update' Works                    │
└─────────────────────────────────────────────────────────────┘

Step 1: Find manifest
▼
.west/config says: manifest is at "my-app/west.yml"
▼
Step 2: Read manifest (my-app/west.yml)
▼
┌─────────────────────────────────────┐
│ projects:                           │
│   - name: zephyr                    │
│     url: github.com/.../zephyr      │
│     revision: v3.7.0                │
│     path: zephyr                    │
│                                     │
│   - name: hal_nordic                │
│     url: github.com/.../hal_nordic  │
│     revision: main                  │
│     path: modules/hal/nordic        │
└─────────────────────────────────────┘
▼
Step 3: Clone/update each project
▼
git clone github.com/.../zephyr → ./zephyr/
git checkout v3.7.0

git clone github.com/.../hal_nordic → ./modules/hal/nordic/
git checkout main
▼
Step 4: Done!
```

### What Makes a Repository a "Manifest Repository"?

```
┌─────────────────────────────────────────────────────────────┐
│           Repository → Manifest Repository                  │
└─────────────────────────────────────────────────────────────┘

BEFORE (Just a regular repository):
my-repo/
├── .git/
├── README.md
├── src/
└── CMakeLists.txt

ADD west.yml:
my-repo/
├── .git/
├── west.yml        ← This file makes it a manifest repo!
├── README.md
├── src/
└── CMakeLists.txt

RUN west init:
$ cd my-repo
$ west init -l .    ← Tell west: "Use THIS repo as manifest"

NOW (Manifest Repository):
my-repo/
├── .git/
├── .west/          ← West workspace created
│   └── config      ← Points to this repo as manifest
├── west.yml        ← The manifest
├── README.md
├── src/
└── CMakeLists.txt

It's now a manifest repository because:
1. ✅ Contains west.yml
2. ✅ West workspace points to it (.west/config)
3. ✅ Controls what gets fetched (west update)
```

---

## Summary

### Manifest Repository = Project Definition File

| Aspect | Explanation |
|--------|-------------|
| **What** | Repository containing `west.yml` |
| **Purpose** | Defines workspace structure and dependencies |
| **Controls** | What to fetch, which versions, where to place |
| **Analogy** | Like `package.json` in Node.js or `requirements.txt` in Python |

### Topology Decision Guide

```
┌─────────────────────────────────────────────────────────────┐
│              Which Topology Should I Use?                   │
└─────────────────────────────────────────────────────────────┘

START
  │
  ├─ Contributing to Zephyr? ──→ YES ──→ T1 (Zephyr-centric)
  │
  ├─ Building one application? ──→ YES ──→ T2 (App-centric) ⭐ RECOMMENDED
  │
  └─ Multiple related apps? ──→ YES ──→ T3 (Forest)


T2 is the industry standard for:
- Commercial products
- Production applications
- Reproducible builds
- Long-term maintenance
```

### Key Takeaway

**The manifest repository is whoever has the `west.yml` file and gets pointed to by `west init -l`.**

- **T1:** Zephyr has the manifest (Zephyr is the boss)
- **T2:** Your app has the manifest (Your app is the boss) ⭐
- **T3:** Separate repo has the manifest (Orchestrator is the boss)

---

**Next Steps:**
- See [GETTING_STARTED.md](GETTING_STARTED.md) for setup instructions
- See [BUILDING.md](BUILDING.md) for build configuration
- See [README.md](../README.md) for quick reference
