package rpc

import (
	"context"
	"strings"

	"ThroneCore/gen"

	"github.com/sagernet/sing-box/adapter"
)

func (s *server) UpdateRuleSets(ctx context.Context, in *gen.EmptyReq) (*gen.UpdateRuleSetsResponse, error) {
	box := currentBox()
	if box == nil {
		return &gen.UpdateRuleSetsResponse{Error: To("no instance is running")}, nil
	}
	var updated int32
	var failures []string
	for _, ruleSet := range box.Router().RuleSets() {
		updatable, ok := ruleSet.(adapter.UpdatableRuleSet)
		if !ok {
			continue
		}
		if err := updatable.Update(); err != nil {
			failures = append(failures, ruleSet.Name()+": "+err.Error())
			continue
		}
		updated++
	}
	return &gen.UpdateRuleSetsResponse{Updated: To(updated), Error: To(strings.Join(failures, "\n"))}, nil
}
