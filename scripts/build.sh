#!/bin/bash

# DEEWALLET Build Script

set -e

echo "🚀 Building DEEWALLET..."

# Clean previous builds
echo "🧹 Cleaning previous builds..."
rm -rf dist/
rm -rf release/

# Install dependencies
echo "📦 Installing dependencies..."
npm install

# Run linter
echo "🔍 Running linter..."
npm run lint

# Run type checker
echo "🔍 Type checking..."
npm run typecheck

# Run tests
echo "🧪 Running tests..."
npm test

# Build main process
echo "🔨 Building main process..."
npm run build:main

# Build renderer process
echo "🔨 Building renderer process..."
npm run build:renderer

# Package application
echo "📦 Packaging application..."
npm run package

echo "✅ Build complete! Check the 'release' folder."
