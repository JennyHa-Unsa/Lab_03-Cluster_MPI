#!/bin/bash
# monitorear.sh - Monitoreo del cluster MPI
# Uso: ./monitorear.sh [intervalo_segundos]

INTERVAL=${1:-5} # Intervalo predeterminado: 5 segundos
HOSTS=("master" "client1" "client2")

echo "=========================================="
echo " MONITOREO CLUSTER MPI - $(date)"
echo " Intervalo: ${INTERVAL}s | Ctrl+C para detener"
echo "=========================================="

while true; do
    clear
    echo "=== $(date '+%H:%M:%S') ==="
    
    for host in "${HOSTS[@]}"; do
        echo ""
        echo "--- $host ---"
        
        # Ejecutar comandos remotos vía SSH
        ssh "$host" '
        # Versión resistente a cambios de formato en top
            echo CPU=$(top -bn1 | grep "Cpu" | sed 's/.* \([0-9]*\)\.[0-9]* id.*/\1/' | awk '{print 100-$1}')
            echo "MEM: $(free -m | awk "/Mem:/ {printf \"%dMB/%dMB (%.1f%%)\", \$3,\$2,\$3*100/\$2}")"
            echo "LOAD: $(uptime | awk -F"load average:" "{print \$2}" | xargs)"
            
            # Temperatura (si sensors está disponible)
            if command -v sensors &> /dev/null; then
                TEMP=$(sensors | grep -i "core 0" | awk "{print \$3}" | tr -d "+°C")
                [ -n "$TEMP" ] && echo "TEMP Core0: ${TEMP}°C" || echo "TEMP: N/A"
            else
                echo "TEMP: sensors no instalado"
            fi
        ' 2>/dev/null || echo "ERROR: No se pudo conectar a $host"
    done
    
    echo ""
    echo "Presiona Ctrl+C para detener el monitoreo"
    sleep "$INTERVAL"
done