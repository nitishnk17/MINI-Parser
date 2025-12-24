#!/bin/bash

# Script to run all test cases for Lab 2B
# Usage: ./run_tests.sh [parser_executable]

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Default parser executable name
PARSER=${1:-"./parser"}

# Check if parser exists
if [ ! -f "$PARSER" ]; then
    echo -e "${RED}Error: Parser executable '$PARSER' not found!${NC}"
    echo "Usage: $0 [parser_executable]"
    echo "Example: $0 ./parser"
    exit 1
fi

# Check if tests directory exists
if [ ! -d "tests" ]; then
    echo -e "${RED}Error: tests/ directory not found!${NC}"
    echo "Please run create_testcases.sh first."
    exit 1
fi

# Counters
valid_passed=0
valid_failed=0
invalid_passed=0
invalid_failed=0

echo -e "${BLUE}================================${NC}"
echo -e "${BLUE}Running Test Suite${NC}"
echo -e "${BLUE}================================${NC}"
echo ""

# Run valid test cases
echo -e "${YELLOW}Testing VALID test cases (should parse successfully):${NC}"
echo ""

if [ -d "tests/valid" ]; then
    for testfile in tests/valid/*.txt; do
        if [ -f "$testfile" ]; then
            testname=$(basename "$testfile")
            
            # Run parser and capture output
            output=$($PARSER < "$testfile" 2>&1)
            exit_code=$?
            
            # Valid tests should succeed (exit code 0)
            if [ $exit_code -eq 0 ]; then
                ((valid_passed++))
            else
                ((valid_failed++))
                echo -e "${RED}✗ FAILED: $testname${NC}"
                echo "  Exit code: $exit_code"
                echo "  Output: $output"
                echo ""
            fi
        fi
    done
else
    echo -e "${RED}Warning: tests/valid/ directory not found${NC}"
fi

# Run invalid test cases
echo -e "${YELLOW}Testing INVALID test cases (should fail gracefully):${NC}"
echo ""

if [ -d "tests/invalid" ]; then
    for testfile in tests/invalid/*.txt; do
        if [ -f "$testfile" ]; then
            testname=$(basename "$testfile")
            
            # Run parser and capture output
            output=$($PARSER < "$testfile" 2>&1)
            exit_code=$?
            
            # Invalid tests should fail (non-zero exit code)
            if [ $exit_code -ne 0 ]; then
                ((invalid_passed++))
            else
                ((invalid_failed++))
                echo -e "${RED}✗ FAILED: $testname (should have failed but passed)${NC}"
                echo "  Exit code: $exit_code"
                echo "  Output: $output"
                echo ""
            fi
        fi
    done
else
    echo -e "${RED}Warning: tests/invalid/ directory not found${NC}"
fi

# Summary
echo -e "${BLUE}================================${NC}"
echo -e "${BLUE}Test Results Summary${NC}"
echo -e "${BLUE}================================${NC}"
echo ""

total_valid=$((valid_passed + valid_failed))
total_invalid=$((invalid_passed + invalid_failed))
total_passed=$((valid_passed + invalid_passed))
total_failed=$((valid_failed + invalid_failed))
total_tests=$((total_valid + total_invalid))

echo -e "${GREEN}Valid test cases passed:   $valid_passed / $total_valid${NC}"
if [ $valid_failed -gt 0 ]; then
    echo -e "${RED}Valid test cases failed:   $valid_failed / $total_valid${NC}"
fi

echo ""
echo -e "${GREEN}Invalid test cases passed: $invalid_passed / $total_invalid${NC}"
if [ $invalid_failed -gt 0 ]; then
    echo -e "${RED}Invalid test cases failed: $invalid_failed / $total_invalid${NC}"
fi

echo ""
echo -e "${BLUE}================================${NC}"
echo -e "${GREEN}Total passed: $total_passed / $total_tests${NC}"
if [ $total_failed -gt 0 ]; then
    echo -e "${RED}Total failed: $total_failed / $total_tests${NC}"
    exit 1
else
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
fi