<?php
session_start();

if (!isset($_SESSION['user'])) {
    header('Location: login.php');
    exit;
}

define('OPTIONS_FILE', __DIR__ . '/options.txt');

// Noms des options
$optionKeys = [
    'charger_ram_off',
    'enregistrer_ram_off',
    'charger_microcode_off',
    'enregistrer_microcode_off',
];

// Valeurs par défaut
$options = array_fill_keys($optionKeys, false);

// Fonction pour lire le fichier INF-like
function readOptionsFile(string $path, array $keys): array {
    $opts = array_fill_keys($keys, false);
    if (!file_exists($path)) return $opts;

    $lines = file($path, FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
    foreach ($lines as $line) {
        $line = trim($line);
        if ($line === '' || str_starts_with($line, '#')) continue;
        if (strpos($line, '=') === false) continue;

        [$key, $value] = explode('=', $line, 2);
        $key = trim($key);
        $value = trim($value);
        if (in_array($key, $keys, true)) {
            $opts[$key] = ($value === '1');
        }
    }
    return $opts;
}

// Fonction pour écrire le fichier INF-like
function writeOptionsFile(string $path, array $opts): void {
    $lines = [
        "# options.txt",
        "# Format INF-like - 1 = interdit, 0 = autorisé",
        ""
    ];
    foreach ($opts as $key => $val) {
        $lines[] = "{$key}=" . ($val ? '1' : '0');
    }
    file_put_contents($path, implode("\n", $lines) . "\n");
}

// Lecture au lancement
$options = readOptionsFile(OPTIONS_FILE, $optionKeys);

$message = '';
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Récupérer les checkbox postés
    foreach ($optionKeys as $key) {
        $options[$key] = isset($_POST[$key]);
    }

    // Écrire dans le fichier
    writeOptionsFile(OPTIONS_FILE, $options);

    $message = 'Options sauvegardées.';
}
?>

<!DOCTYPE html>
<html lang="fr">
<head>
<meta charset="UTF-8" />
<title>Dashboard - Options</title>
<style>
/* même style que précédemment, je ne remets pas ici pour la brièveté */
body {
    font-family: Arial, sans-serif;
    background: #e9ecef;
    padding: 2rem;
}
.container {
    max-width: 600px;
    margin: auto;
    background: white;
    padding: 2rem;
    border-radius: 8px;
    box-shadow: 0 0 10px rgba(0,0,0,0.1);
}
h1 {
    margin-bottom: 1.5rem;
    color: #222;
}
fieldset {
    border: 1px solid #ccc;
    padding: 1rem 1.5rem;
    margin-bottom: 1.5rem;
    border-radius: 6px;
}
legend {
    font-weight: bold;
    padding: 0 0.5rem;
    color: #444;
}
label {
    display: block;
    margin-bottom: 0.75rem;
    cursor: pointer;
    user-select: none;
}
input[type="checkbox"] {
    margin-right: 0.5rem;
    transform: scale(1.2);
    vertical-align: middle;
}
select {
    width: 100%;
    padding: 0.4rem 0.5rem;
    font-size: 1rem;
    border-radius: 6px;
    border: 1px solid #ccc;
}
.buttons {
    display: flex;
    justify-content: space-between;
    margin-top: 2rem;
}
button {
    padding: 0.7rem 1.4rem;
    font-size: 1rem;
    border: none;
    border-radius: 6px;
    cursor: pointer;
    color: white;
    background-color: #007bff;
    transition: background-color 0.3s ease;
}
button:hover {
    background-color: #0056b3;
}
button.logout {
    background-color: #dc3545;
}
button.logout:hover {
    background-color: #a71d2a;
}
.message {
    margin-bottom: 1rem;
    color: green;
    font-weight: bold;
}
</style>
</head>
<body>
<div class="container" role="main">
    <h1>Options</h1>

    <?php if ($message): ?>
        <div class="message" role="alert"><?= htmlspecialchars($message) ?></div>
    <?php endif; ?>

    <form method="post" action="">
        <fieldset>
            <legend>Actions</legend>
            <label for="actions_select">Sélectionnez une action :</label>
            <select id="actions_select" name="actions_select" onchange="handleActionChange()">
                <option value="normal">Tout autoriser (mode normal)</option>
                <option value="examen">Tout interdire (mode examen)</option>
            </select>
        </fieldset>

        <fieldset id="checkbox_block">
            <legend>Options d'interdiction</legend>
            <label><input type="checkbox" name="charger_ram_off" id="charger_ram_off" <?= $options['charger_ram_off'] ? 'checked' : '' ?>> Interdire Charger Ram</label>
            <label><input type="checkbox" name="enregistrer_ram_off" id="enregistrer_ram_off" <?= $options['enregistrer_ram_off'] ? 'checked' : '' ?>> Interdire Enregistrer Ram</label>
            <label><input type="checkbox" name="charger_microcode_off" id="charger_microcode_off" <?= $options['charger_microcode_off'] ? 'checked' : '' ?>> Interdire Charger Table Microcode</label>
            <label><input type="checkbox" name="enregistrer_microcode_off" id="enregistrer_microcode_off" <?= $options['enregistrer_microcode_off'] ? 'checked' : '' ?>> Interdire Enregistrer Table Microcode</label>
        </fieldset>

        <div class="buttons">
            <button type="submit">Valider les options</button>
            <button type="button" class="logout" onclick="location.href='logout.php'">Logout</button>
        </div>
    </form>
</div>

<script>
function handleActionChange() {
    const select = document.getElementById('actions_select');
    const checkedBoxes = document.querySelectorAll('#checkbox_block input[type=checkbox]');
    if (select.value === 'normal') {
        // Tout autoriser => décocher et désactiver checkbox
        checkedBoxes.forEach(cb => {
            cb.checked = false;
            cb.disabled = true;
        });
    } else if (select.value === 'examen') {
        // Tout interdire => cocher et activer checkbox
        checkedBoxes.forEach(cb => {
            cb.checked = true;
            cb.disabled = false;
        });
    }
}

// Au chargement, synchroniser le select avec l'état des checkbox
function syncSelectWithCheckboxes() {
    const select = document.getElementById('actions_select');
    const checkedBoxes = document.querySelectorAll('#checkbox_block input[type=checkbox]');
    const allChecked = Array.from(checkedBoxes).every(cb => cb.checked);
    const allUnchecked = Array.from(checkedBoxes).every(cb => !cb.checked);

    if (allChecked) {
        select.value = 'examen';
        checkedBoxes.forEach(cb => cb.disabled = false);
    } else if (allUnchecked) {
        select.value = 'normal';
        checkedBoxes.forEach(cb => cb.disabled = true);
    } else {
        // Cas mixte : on active checkboxes pour permettre modification
        select.value = '';
        checkedBoxes.forEach(cb => cb.disabled = false);
    }
}

window.addEventListener('DOMContentLoaded', () => {
    syncSelectWithCheckboxes();

    const checkboxes = document.querySelectorAll('#checkbox_block input[type=checkbox]');
    checkboxes.forEach(cb => {
        cb.addEventListener('change', () => {
            syncSelectWithCheckboxes();
        });
    });
});
</script>
</body>
</html>

