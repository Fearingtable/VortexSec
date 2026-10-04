// =============================================================================
// VortexSec :: CWE-416 Use-After-Free
// -----------------------------------------------------------------------------
// Dimostra come un dangling pointer possa portare alla sovrascrittura di una
// VTable (o di qualunque struttura dati rilocata nello stesso heap chunk),
// con conseguente possibile dirottamento del flusso di controllo.
// Compilare con: clang++ -fsanitize=address -g -std=c++20 uaf_example.cpp
// =============================================================================

#include <cstdio>
#include <memory>

// -----------------------------------------------------------------------------
// VERSIONE VULNERABILE
// -----------------------------------------------------------------------------
struct Widget {
    virtual void render() { std::printf("Widget::render (safe)\n"); }
    virtual ~Widget() = default;
};

void vulnerable_uaf_demo() {
    Widget* w = new Widget();
    w->render();

    delete w;                 // (1) la memoria viene liberata, ma il puntatore
                               //     resta "vivo" (dangling pointer)

    // (2) Un allocatore potrebbe riutilizzare lo stesso chunk per un oggetto
    //     di tipo diverso e dimensione compatibile, ad es. un buffer
    //     controllato da un attaccante contenente una VTable fasulla.
    //     La chiamata seguente è undefined behavior: con ASan va in crash
    //     controllato; senza mitigazioni può eseguire codice arbitrario
    //     se il chunk è stato "grooming-ato" ad arte.
    w->render();               // <-- USE-AFTER-FREE
}

// -----------------------------------------------------------------------------
// VERSIONE CORRETTA — RAII + std::unique_ptr (C++20)
// -----------------------------------------------------------------------------
// Il possesso esclusivo della risorsa è espresso nel tipo stesso: quando
// unique_ptr esce dallo scope, il distruttore libera la memoria una sola
// volta e in modo deterministico. Non esiste più un puntatore "nudo" che
// possa sopravvivere alla deallocazione.
void fixed_raii_demo() {
    auto w = std::make_unique<Widget>();
    w->render();

    // Nessun delete manuale necessario: RAII garantisce la deallocazione
    // automatica a fine scope, eliminando strutturalmente la classe di bug.
}   // <-- qui il distruttore di Widget viene invocato automaticamente

// -----------------------------------------------------------------------------
// VARIANTE: condivisione sicura con std::shared_ptr quando il possesso
// non è esclusivo (es. più componenti osservano lo stesso oggetto).
// -----------------------------------------------------------------------------
void fixed_shared_demo() {
    std::shared_ptr<Widget> w1 = std::make_shared<Widget>();
    std::weak_ptr<Widget> observer = w1;   // riferimento non proprietario

    if (auto locked = observer.lock()) {   // verifica di validità esplicita
        locked->render();
    }
}

int main() {
    std::puts("=== Fixed: RAII demo ===");
    fixed_raii_demo();

    std::puts("=== Fixed: shared_ptr/weak_ptr demo ===");
    fixed_shared_demo();

    // La riga seguente è commentata di default: decommentare SOLO sotto
    // AddressSanitizer per osservare il crash controllato e il report
    // "heap-use-after-free" con relativo stack trace di allocazione/free.
    // std::puts("=== Vulnerable: UAF demo ===");
    // vulnerable_uaf_demo();

    return 0;
}
