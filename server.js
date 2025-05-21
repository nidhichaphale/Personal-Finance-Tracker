const express = require('express');
const mongoose = require('mongoose');
const path = require('path');
const bcrypt = require('bcrypt');
const jwt = require('jsonwebtoken');

const app = express();

const JWT_SECRET = 'your_jwt_secret_key'; // Change to a strong secret in production
const JWT_EXPIRES_IN = '1h';           // Token expiration

// Connect to MongoDB
mongoose.connect('mongodb://localhost:27017/finance-tracker')
  .then(() => console.log('✅ Connected to MongoDB'))
  .catch(err => console.error('❌ MongoDB connection error:', err));

// Debug middleware: logs every incoming request
app.use((req, res, next) => {
  console.log(`👉 ${req.method} ${req.url}`);
  next();
});

// Body parsing
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

// Simple test route to confirm POST requests are reaching the server
app.post('/test', (req, res) => {
  console.log('🎯 /test hit with body:', req.body);
  res.json({ message: 'Test route is working', data: req.body });
});

// Define User schema and model
const userSchema = new mongoose.Schema({
  email:       { type: String, required: true, unique: true },
  passwordHash:{ type: String, required: true },
});
const User = mongoose.model('User', userSchema);

// ─── Registration ───────────────────────────────────────────────────────────────
app.post('/api/register', async (req, res) => {
  console.log('🎯 /api/register');
  const { email, password } = req.body;

  try {
    if (!email || !password) {
      return res.status(400).json({ message: 'Email and password are required' });
    }

    const exists = await User.findOne({ email });
    if (exists) {
      return res.status(409).json({ message: 'Email already registered' });
    }

    const passwordHash = await bcrypt.hash(password, 10);
    await new User({ email, passwordHash }).save();
    console.log('✅ Registration successful');

    res.status(201).json({ message: 'User registered successfully' });
  } catch (err) {
    console.error('🔥 Error in /api/register:', err);
    res.status(500).json({ message: 'Server error' });
  }
});

// ─── Login ──────────────────────────────────────────────────────────────────────
app.post('/api/login', async (req, res) => {
  console.log('🎯 /api/login');

  const { email, password } = req.body;
  console.log(`🟡 Login attempt with email: ${email} | password: ${password}`);

  try {
    if (!email || !password) {
      return res.status(400).json({ message: 'Email and password are required' });
    }

    const user = await User.findOne({ email });
    console.log('🔍 Found user:', user);

    if (!user) {
      console.log('❌ User not found in DB');
      return res.status(401).json({ message: 'Invalid credentials' });
    }

    const match = await bcrypt.compare(password, user.passwordHash);
    console.log('🔐 Password match:', match);

    if (!match) {
      console.log('❌ Password did not match');
      return res.status(401).json({ message: 'Invalid credentials' });
    }

    const token = jwt.sign(
      { userId: user._id, email: user.email },
      JWT_SECRET,
      { expiresIn: JWT_EXPIRES_IN }
    );

    console.log('✅ Login successful');
    res.json({ message: 'Login successful', token, redirect: '/dashboard.html' });

  } catch (err) {
    console.error('🔥 Error in /api/login:', err);
    res.status(500).json({ message: 'Server error' });
  }
});

// ─── Token Validation ───────────────────────────────────────────────────────────
app.get('/api/validate-token', (req, res) => {
  const auth = req.headers.authorization || '';
  if (!auth.startsWith('Bearer ')) {
    return res.status(401).json({ message: 'Authorization token missing' });
  }

  const token = auth.split(' ')[1];
  try {
    const decoded = jwt.verify(token, JWT_SECRET);
    res.json({ valid: true, user: decoded });
  } catch {
    res.status(401).json({ message: 'Invalid or expired token' });
  }
});

// ─── Static & HTML Routes ──────────────────────────────────────────────────────
app.use(express.static(path.join(__dirname, 'public')));

// Set '/' route to serve About page first
app.get('/', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'aboutpage.html'));
});

// Route for login page
app.get('/login', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'login.html'));
});

// Route for dashboard
app.get('/dashboard.html', (req, res) => {
  res.sendFile(path.join(__dirname, 'public', 'dashboard.html'));
});

// 404 handler
app.use((req, res) => {
  res.status(404).send('❓ Not Found');
});

// Start server
app.listen(3000, () => {
  console.log('🚀 Server running at http://localhost:3000');
});
