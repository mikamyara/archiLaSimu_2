<?php
// login.php

session_start();

$users = [
    'admin' => '$2y$10$KhQXBETpDHW.KRKOV7RUg./b1QHvp4GFDuzm8TvXFYOz1AZR33Cai' // password = "password123"
];

function canLoginAgain() {
    if (isset($_SESSION['last_login_time'])) {
        $elapsed = time() - $_SESSION['last_login_time'];
        return $elapsed >= 2;
    }
    return true;
}

$error = '';

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $username = $_POST['username'] ?? '';
    $password = $_POST['password'] ?? '';

    if (!canLoginAgain()) {
        $error = "Merci d'attendre un peu avant de tenter une nouvelle connexion.";
    } elseif (isset($users[$username]) && password_verify($password, $users[$username])) {
        $_SESSION['user'] = $username;
        $_SESSION['last_login_time'] = time();
        session_regenerate_id(true);
        header("Location: dashboard.php");
        exit;
    } else {
        $error = "Identifiant ou mot de passe incorrect.";
        $_SESSION['last_login_time'] = time();
    }
}
?>
<!DOCTYPE html>
<html lang="fr">
<head>
<meta charset="UTF-8" />
<title>Login sécurisé</title>
<style>
    body {
        font-family: Arial, sans-serif;
        background: #f0f2f5;
        display: flex;
        justify-content: center;
        align-items: center;
        height: 100vh;
    }
    .login-box {
        background: white;
        padding: 2rem;
        border-radius: 8px;
        box-shadow: 0 2px 10px rgba(0,0,0,0.1);
        width: 320px;
    }
    h2 {
        text-align: center;
        margin-bottom: 1.5rem;
        color: #333;
    }
    label {
        display: block;
        margin-bottom: .5rem;
        font-weight: bold;
        color: #555;
    }
    input[type="text"], input[type="password"] {
        width: 100%;
        padding: .5rem;
        margin-bottom: 1rem;
        border: 1px solid #ccc;
        border-radius: 4px;
        font-size: 1rem;
    }
    button {
        width: 100%;
        padding: .7rem;
        background: #007bff;
        color: white;
        border: none;
        border-radius: 4px;
        font-size: 1rem;
        cursor: pointer;
        transition: background .3s ease;
    }
    button:hover {
        background: #0056b3;
    }
    .error {
        color: #d9534f;
        margin-bottom: 1rem;
        text-align: center;
        font-weight: bold;
    }
</style>
</head>
<body>
<div class="login-box">
    <h2>Connexion</h2>
    <?php if ($error): ?>
        <div class="error"><?= htmlspecialchars($error) ?></div>
    <?php endif; ?>
    <form method="post" action="">
        <label for="username">Utilisateur</label>
        <input type="text" id="username" name="username" required autofocus autocomplete="username" />

        <label for="password">Mot de passe</label>
        <input type="password" id="password" name="password" required autocomplete="current-password" />

        <button type="submit">Se connecter</button>
    </form>
</div>
</body>
</html>

