# VortexSec

### C/C++ Low-Level Vulnerability & Malware Mechanics Analysis Lab

![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/build-CMake-064F8C?logo=cmake&logoColor=white)
![License](https://img.shields.io/badge/license-MIT-green)
![Build](https://img.shields.io/badge/build-passing-brightgreen)
![ASan](https://img.shields.io/badge/AddressSanitizer-enabled-orange)

---

## ⚠️ Disclaimer Legale ed Etico

Questo repository è un **progetto didattico** creato a scopo esclusivamente educativo, nell'ambito di un percorso universitario di Ingegneria Informatica incentrato sulla sicurezza del software a basso livello.

- Tutto il codice relativo a vulnerabilità e meccanismi offensivi è **isolato, non armato e privo di payload reali**: non contiene logica di persistenza, propagazione, comando e controllo (C2) o danneggiamento di dati reali.
- Gli esempi vanno eseguiti **esclusivamente in ambienti isolati** (VM dedicate, container, sandbox senza accesso di rete o con rete air-gapped).
- L'autore **non si assume alcuna responsabilità** per un uso improprio del codice. Questo materiale non deve essere impiegato per sviluppare, distribuire o testare software malevolo su sistemi non espressamente autorizzati.
- L'uso di queste tecniche contro sistemi senza autorizzazione esplicita e scritta del proprietario è **illegale** nella maggior parte delle giurisdizioni (in Italia: artt. 615-ter, 635-bis c.p. e normativa correlata).
- Il progetto è concepito in linea con i principi della **Responsible Disclosure** e della ricerca di sicurezza etica (ethical hacking / red team didattico).

Procedendo nella lettura o nell'uso del codice, l'utente dichiara di accettare questi termini.

---

## 🎯 Obiettivo del Progetto

VortexSec esplora, a livello di codice sorgente C/C++, tre dimensioni della sicurezza del software:

1. **Offensive mechanics** — come nascono concretamente le vulnerabilità memory-safety più sfruttate (CWE Top 25).
2. **Malware internals** — perché certe classi di malware scelgono C/C++, con analisi architetturale (non operativa) delle tecniche usate.
3. **Defensive hardening** — come neutralizzare strutturalmente queste classi di bug con RAII, smart pointer e strumenti di analisi automatica.

---

## 🧩 Architecture & Vulnerabilities Covered

| Modulo | CWE | Tecnica difensiva dimostrata |
|---|---|---|
| Buffer Overflow (stack/heap) | CWE-787 / CWE-121 / CWE-122 | Bound checking, `std::span`, ASan |
| Use-After-Free | CWE-416 | RAII, `std::unique_ptr` / `std::weak_ptr` |
| Integer Overflow | CWE-190 | Controlli espliciti, `std::vector` safe API |
| Format String | CWE-134 | Format string literal fissi, wrapper type-safe |
| Cifratura ibrida (analisi ransomware) | — | Isolamento del modulo, nessuna persistenza |

Dettagli completi in [`docs/ARCHITECTURE.md`](docs/ARCHITECTURE.md) e nei singoli report in [`docs/VULNERABILITY_REPORTS/`](docs/VULNERABILITY_REPORTS/).

---

## 🏗️ Struttura del Repository

```
VortexSec/
├── docs/                      # Architettura, threat model, report per vulnerabilità
├── include/                   # Header pubblici per ciascun modulo
├── src/
│   ├── vulnerabilities/       # Esempi vulnerabili + patch
│   ├── malware_analysis/      # Moduli teorico-dimostrativi isolati
│   └── defenses/              # Wrapper sicuri, sanitizzazione input
├── tests/                     # Unit test (GoogleTest / Catch2)
└── rules/                     # Regole YARA di rilevamento
```

---

## ⚙️ Build Instructions

### Requisiti
- CMake ≥ 3.20
- Compilatore con supporto C++20 (Clang ≥ 14 consigliato per Clang Static Analyzer)
- OpenSSL 3.x (per il modulo di cifratura)

### Build standard
```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Build con AddressSanitizer (consigliata per lo sviluppo)
```bash
mkdir build-asan && cd build-asan
cmake -DCMAKE_CXX_FLAGS="-fsanitize=address -fno-omit-frame-pointer -g" ..
cmake --build .
```

### Analisi statica con Clang
```bash
scan-build cmake --build build
```

### Analisi dinamica con Valgrind
```bash
valgrind --leak-check=full --show-leak-kinds=all ./build/vortexsec
```

---

## 🔁 How to Reproduce & Mitigation Steps

Ogni modulo in `src/vulnerabilities/` segue questa struttura:

1. **Funzione vulnerabile** — isolata e commentata, compilabile ma mai eseguita di default nel `main()`.
2. **Trigger controllato** — attivabile solo tramite una macro o un flag esplicito, mai in build di produzione.
3. **Report in `docs/VULNERABILITY_REPORTS/`** — spiega causa radice, impatto teorico (CVSS), e come riprodurre il crash sotto ASan/Valgrind.
4. **Patch dimostrativa** — stessa funzionalità riscritta con pattern sicuro (RAII, bound checking, ecc.).

Per riprodurre un singolo caso di studio:
```bash
./build/vortexsec --demo uaf      # esegue solo la versione corretta per default
```

---

## 📜 Licenza

Distribuito sotto licenza MIT — vedi [`LICENSE`](LICENSE).
