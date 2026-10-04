# VortexSec

### C/C++ Low-Level Vulnerability & Malware Mechanics Analysis Lab

![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=c%2B%2B&logoColor=white)
![License](https://img.shields.io/badge/license-MIT-green)
![ASan](https://img.shields.io/badge/AddressSanitizer-ready-orange)
![Docs](https://img.shields.io/badge/docs-PDF%20report-blue)

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

📄 **[Leggi il report completo (PDF)](docs/VULNERABILITY_REPORTS/vulnerability_report.pdf)** — analisi dettagliata di 4 CVE reali (EternalBlue, BlueKeep, CVE-2006-5270, CVE-2019-1579), con causa radice, malware/ransomware associati e contromisure per ciascuna.

---

## 🧩 Moduli Disponibili

| Modulo | CWE | Cosa dimostra |
|---|---|---|
| [`src/vulnerabilities/uaf_example.cpp`](src/vulnerabilities/uaf_example.cpp) | CWE-416 (Use-After-Free) | Dangling pointer su oggetto polimorfico → patch con RAII (`std::unique_ptr`, `std::weak_ptr`) |
| [`src/malware_analysis/hybrid_crypto_demo.cpp`](src/malware_analysis/hybrid_crypto_demo.cpp) | — (architettura crittografica) | Schema di cifratura ibrida AES-256-GCM + RSA-2048-OAEP tipico dei ransomware, isolato e su dati di test locali |

Nuovi moduli (buffer overflow, integer overflow, format string) verranno aggiunti qui seguendo la stessa struttura: versione vulnerabile commentata + patch, come già descritto nel [report](docs/vulnerability_report.pdf).

---

## 🏗️ Struttura del Repository

```
VortexSec/
├── LICENSE
├── .gitignore
├── src/
│   ├── vulnerabilities/       # Esempi vulnerabili e relative patch
│   └── malware_analysis/      # Moduli teorico-dimostrativi isolati
└── docs/
    └── VULNERABILITY_REPORTS/
        └── vulnerability_report.pdf
```

---

## ⚙️ Come Compilare ed Eseguire

Ogni modulo è attualmente un file singolo, autocontenuto e compilabile in isolamento — nessun sistema di build necessario per ora.

### `uaf_example.cpp`
Richiede Clang (per AddressSanitizer) e supporto C++20.
```bash
clang++ -fsanitize=address -g -std=c++20 src/vulnerabilities/uaf_example.cpp -o uaf_demo
./uaf_demo
```

### `hybrid_crypto_demo.cpp`
Richiede OpenSSL 3.x (`libcrypto`).
```bash
g++ -std=c++20 src/malware_analysis/hybrid_crypto_demo.cpp -lcrypto -o hybrid_demo
./hybrid_demo
```

> Man mano che il numero di moduli cresce, questa sezione verrà sostituita da un `CMakeLists.txt` unico con target separati per ciascun modulo.

---

## 📜 Licenza

Distribuito sotto licenza MIT — vedi [`LICENSE`](LICENSE).
