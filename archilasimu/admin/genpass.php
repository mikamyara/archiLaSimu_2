<?php
// genpass.php
// Simple : entre un mot de passe, clique "Générer", on affiche le hash bcrypt.

$hash = '';
$err = '';

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $pwd = $_POST['password'] ?? '';

    if ($pwd === '') {
        $err = 'Veuillez saisir un mot de passe.';
    } else {
        // Génère un hash sécurisé (bcrypt)
        $hash = password_hash($pwd, PASSWORD_DEFAULT);
        if ($hash === false) {
            $err = 'Erreur lors de la génération du hash.';
        }
    }
}
?>
<!doctype html>
<html lang="fr">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>Générateur de mot de passe (hash)</title>
<style>
    :root{
        --bg:#f5f7fb;
        --card:#fff;
        --accent:#4f46e5;
        --muted:#6b7280;
    }
    body{
        margin:0;
        font-family: Inter, system-ui, -apple-system, "Segoe UI", Roboto, "Helvetica Neue", Arial;
        background: linear-gradient(180deg, #eef2ff 0%, var(--bg) 100%);
        min-height:100vh;
        display:flex;
        align-items:center;
        justify-content:center;
        padding:24px;
    }
    .card{
        width:100%;
        max-width:520px;
        background:var(--card);
        border-radius:12px;
        box-shadow:0 6px 30px rgba(15,23,42,0.08);
        padding:20px;
    }
    h1{
        margin:0 0 8px 0;
        font-size:20px;
        color:#0f172a;
    }
    p.lead{
        margin:0 0 18px 0;
        color:var(--muted);
        font-size:14px;
    }
    label{display:block;font-size:13px;color:#334155;margin-bottom:6px;}
    input[type="text"], input[type="password"]{
        width:100%;
        padding:10px 12px;
        border-radius:8px;
        border:1px solid #e6e9ef;
        font-size:15px;
        box-sizing:border-box;
        background:#fbfdff;
    }
    .row{display:flex;gap:10px;margin-top:12px;}
    button{
        background:var(--accent);
        color:white;
        border:0;
        padding:10px 14px;
        border-radius:8px;
        cursor:pointer;
        font-weight:600;
    }
    button.secondary{
        background:#eef2ff;
        color:var(--accent);
        border:1px solid rgba(79,70,229,0.12);
    }
    .result{
        margin-top:16px;
    }
    .help{font-size:13px;color:var(--muted);margin-top:8px;}
    .error{color:#b91c1c;font-weight:600;margin-top:8px;}
    .copy-btn{
        background:#111827;
        color:white;
        border-radius:8px;
        padding:8px 10px;
        cursor:pointer;
        border:0;
    }
    .flex-between{display:flex;align-items:center;justify-content:space-between;gap:12px;}
    @media (max-width:520px){
        .row{flex-direction:column;}
    }
</style>
</head>
<body>
<div class="card" role="main">
    <h1>Générateur de mot de passe (hash)</h1>
    <p class="lead">Entre un mot de passe dans le champ ci‑dessous et clique <strong>Générer</strong>. Le hash (bcrypt) sera produit et affiché.</p>

    <form method="post" action="">
        <label for="password">Mot de passe</label>
        <input id="password" name="password" type="password" autocomplete="new-password" placeholder="Tape ton mot de passe ici">

        <div class="row" style="margin-top:12px;">
            <button type="submit">Générer</button>
            <button type="button" class="secondary" onclick="document.getElementById('password').value=''">Effacer</button>
        </div>
    </form>

    <?php if ($err): ?>
        <div class="error" role="alert"><?= htmlspecialchars($err) ?></div>
    <?php endif; ?>

    <?php if ($hash): ?>
        <div class="result">
            <div class="flex-between">
                <div>
                    <label for="out">Hash généré (bcrypt)</label>
                    <input id="out" type="text" readonly value="<?= htmlspecialchars($hash) ?>" onclick="this.select()" />
                </div>
                <div style="display:flex;flex-direction:column;gap:8px;">
                    <button class="copy-btn" onclick="copyHash()">Copier</button>
                    <button class="secondary" onclick="showPlain()">Voir mot de passe</button>
                </div>
            </div>
            <p class="help">Ce hash peut être stocké dans une base de données et vérifié avec <code>password_verify()</code> lors de la connexion.</p>
        </div>
    <?php endif; ?>
</div>

<script>
function copyHash(){
    const out = document.getElementById('out');
    if(!out) return;
    navigator.clipboard?.writeText(out.value).then(()=>{
        alert('Hash copié dans le presse-papiers.');
    }).catch(()=>{
        // fallback
        out.select();
        try { document.execCommand('copy'); alert('Hash copié (fallback).'); }
        catch(e){ alert('Impossible de copier automatiquement. Sélectionne et copie manuellement.'); }
    });
}

function showPlain(){
    // ATTENTION : pour simplicité (UX), on affiche une alerte demandant le mot de passe saisi.
    const pwd = document.getElementById('password').value;
    if(!pwd) { alert("Aucun mot de passe saisi dans le champ."); return; }
    // On propose d'ouvrir une petite boîte pour afficher (client-side only)
    const w = window.open("", "_blank", "width=600,height=200");
    w.document.write("<pre style='font-family:monospace;padding:20px;'>"+escapeHtml(pwd)+"</pre>");
}

function escapeHtml(s){
    return s.replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;');
}
</script>
</body>
</html>

